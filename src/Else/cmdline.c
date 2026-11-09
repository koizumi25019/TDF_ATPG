//------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<limits.h>
#include<ctype.h>
#include<time.h>

//------------------------------------------------------------------------
//自作インクルード
//------------------------------------------------------------------------
#include"../Header/Define.h"

//------------------------------------------------------------------------
//使い方の表示
//------------------------------------------------------------------------
static void usage(void) {
	printf("\n");
	printf("【Usage】 TDF_ATPG -net <circuit.v> -power <ON|OFF> -folder <output_dir/> [options]\n");
	printf("  -net <file>            回路ファイル(必須)\n");
	printf("  -power <ON|OFF>        低消費電力指向(必須)\n");
	printf("  -folder <dir>          出力フォルダー(必須。既に存在するとエラー)\n");
	printf("  -fault <file>          故障ファイル(省略時は全故障)\n");
	printf("  -untest_mode <ON|OFF>  テスト不能故障削除(default:ON)\n");
	printf("  -rand <ON|OFF>         ランダム目標故障選択(default:OFF)\n");
	printf("  -time <sec>            Solver打ち切り時間 0以上(default:10)\n");
	printf("  -wsath <percent>       WSA閾値 0-100(default:20)\n");
	printf("  -target_limit <n>      目標故障数の上限 0以上(0:制限なし)\n");
	printf("  -seed <n>              乱数の種 0以上(省略時は実行時刻。同じ値なら同じ結果になる)\n");
	printf("  -threads <n>           clasp の並列数 1-1024(default:24)\n");
}

//------------------------------------------------------------------------
//引数がオプション名かどうか(「値の書き忘れ」の検出用)
//------------------------------------------------------------------------
static int is_option_name(const char* s) {
	static const char* names[] = { "-net", "-power", "-folder", "-untest_mode", "-rand",
								   "-time", "-wsath", "-target_limit", "-fault", "-seed", "-threads" };
	int k;
	for (k = 0; k < (int)(sizeof(names) / sizeof(names[0])); k++) {
		if (strcmp(s, names[k]) == 0) return 1;
	}
	return 0;
}

//------------------------------------------------------------------------
//オプション(argv[i])の値を取り出す。値が無い/次のオプション名だった場合は NULL
//------------------------------------------------------------------------
static char* get_value(int argc, char* argv[], int i) {
	if (i + 1 >= argc || is_option_name(argv[i + 1])) {
		printf("Option【%s】に値がありません\n", argv[i]);
		return NULL;
	}
	return argv[i + 1];
}

//------------------------------------------------------------------------
//大文字小文字を区別しない比較(一致:1)
//------------------------------------------------------------------------
static int equal_nocase(const char* a, const char* b) {
	while (*a != '\0' && *b != '\0') {
		if (toupper((unsigned char)*a) != toupper((unsigned char)*b)) return 0;
		a++; b++;
	}
	return *a == *b;
}

//------------------------------------------------------------------------
//ON/OFF の値を検証し、"ON"/"OFF" のどちらかに正規化して *out へ格納
//------------------------------------------------------------------------
static int parse_on_off(const char* opt, const char* val, char** out) {
	if (equal_nocase(val, "ON"))  { *out = "ON";  return SUCCESS; }
	if (equal_nocase(val, "OFF")) { *out = "OFF"; return SUCCESS; }
	printf("Option【%s】の値【%s】が不正です。ON か OFF を指定してください\n", opt, val);
	return ERROR;
}

//------------------------------------------------------------------------
//整数の値を検証(全体が数値で、min-maxの範囲内であること)
//------------------------------------------------------------------------
static int parse_int(const char* opt, const char* val, int min, int max, int* out) {
	char* end;
	long v;

	errno = 0;
	v = strtol(val, &end, 10);
	if (val[0] == '\0' || *end != '\0' || errno == ERANGE || v < min || v > max) {
		printf("Option【%s】の値【%s】が不正です。%d-%d の整数を指定してください\n", opt, val, min, max);
		return ERROR;
	}
	*out = (int)v;
	return SUCCESS;
}

//------------------------------------------------------------------------
//読み込み用ファイルが開けるかの確認
//------------------------------------------------------------------------
static int check_readable(const char* opt, const char* path) {
	FILE* fp = fopen(path, "r");
	if (fp == NULL) {
		printf("Option【%s】のファイル【%s】を開けません\n", opt, path);
		return ERROR;
	}
	fclose(fp);
	return SUCCESS;
}

//===================================================
//関数名:cmdline
//機能:コマンドライン解析(値の抜け・不正な値・存在しないファイルをここで検出する)
//戻り値：int 正常終了:SUCCESS(=0) , 異常終了:ERROR(=-1)
//引数：コマンドライン引数の数, コマンドライン引数
//===================================================
int cmdline(int argc, char *argv[])
{
	int i;
	char* val;

	if (argc <= 1) {
		usage();
		return ERROR;
	}

	//------------------------------------------------------------------------
	// 実行コマンド解析
	//------------------------------------------------------------------------
	for (i = 1; i < argc; i++)
	{
		/* 未定義のオプションが指定された場合 */
		if (!is_option_name(argv[i])) {
			printf("Option【%s】is not defined! 未定義のオプションが定義されました！\n", argv[i]);
			usage();
			return ERROR;
		}

		/* 以降は全て値を1つ取るオプション */
		if ((val = get_value(argc, argv, i)) == NULL) { usage(); return ERROR; }

		/* 回路ファイル名の登録 */
		if (!strcmp(argv[i], "-net")) {
			fi[CIRCUIT].name = val;
			fi[CIRCUIT].flag = ON;
		}
		/* 低消費電力モード取得 */
		else if (!strcmp(argv[i], "-power")) {
			if (parse_on_off(argv[i], val, &fi[LOW_POWER].name) != SUCCESS) { usage(); return ERROR; }
			fi[LOW_POWER].flag = ON;
		}
		/* フォルダーの指定 */
		else if (!strcmp(argv[i], "-folder")) {
			fi[FOLDER].name = val;
			fi[FOLDER].flag = ON;
		}
		/* テスト不能故障削除モード(default:ON)取得 */
		else if (!strcmp(argv[i], "-untest_mode")) {
			if (parse_on_off(argv[i], val, &fi[UNTEST_MODE].name) != SUCCESS) { usage(); return ERROR; }
			fi[UNTEST_MODE].flag = ON;
		}
		/* ランダム目標故障選択(default:OFF)取得 */
		else if (!strcmp(argv[i], "-rand")) {
			if (parse_on_off(argv[i], val, &fi[RAND].name) != SUCCESS) { usage(); return ERROR; }
			fi[RAND].flag = ON;
		}
		/* Solver打ち切り時間(default:10)取得 */
		else if (!strcmp(argv[i], "-time")) {
			if (parse_int(argv[i], val, 0, INT_MAX, &fi[TIME].value) != SUCCESS) { usage(); return ERROR; }
			fi[TIME].flag = ON;
		}
		/* WSA閾値(低消費電力閾値)(default:20)取得 */
		else if (!strcmp(argv[i], "-wsath")) {
			if (parse_int(argv[i], val, 0, 100, &fi[WSATH].value) != SUCCESS) { usage(); return ERROR; }
			fi[WSATH].flag = ON;
		}
		/* 目標故障の上限取得(0の場合は制限なし) */
		else if (!strcmp(argv[i], "-target_limit")) {
			if (parse_int(argv[i], val, 0, INT_MAX, &fi[TARGET_LIMIT].value) != SUCCESS) { usage(); return ERROR; }
			fi[TARGET_LIMIT].flag = ON;
		}
		/* 乱数の種取得 */
		else if (!strcmp(argv[i], "-seed")) {
			if (parse_int(argv[i], val, 0, INT_MAX, &fi[SEED].value) != SUCCESS) { usage(); return ERROR; }
			fi[SEED].flag = ON;
		}
		/* clasp の並列数取得 */
		else if (!strcmp(argv[i], "-threads")) {
			if (parse_int(argv[i], val, 1, 1024, &fi[THREADS].value) != SUCCESS) { usage(); return ERROR; }
			fi[THREADS].flag = ON;
		}
		/* 故障ファイル取得 */
		else if (!strcmp(argv[i], "-fault")) {
			fi[FAULT].name = val;
			fi[FAULT].flag = ON;
		}

		i++;	//値の分を読み飛ばす
	}

	//------------------------------------------------------------------------
	// 必要コマンドの入力確認
	//------------------------------------------------------------------------
	if (fi[CIRCUIT].flag == OFF) { printf("回路ファイル名を入力してください(-net)\n");		usage(); return ERROR; }
	if (fi[LOW_POWER].flag == OFF) { printf("低消費電力指向か否かを入力してください(-power)\n");	usage(); return ERROR; }
	if (fi[FOLDER].flag == OFF) { printf("出力フォルダー名を入力してください(-folder)\n");		usage(); return ERROR; }
	if (fi[FOLDER].name[0] == '\0') { printf("Option【-folder】が空です\n");	usage(); return ERROR; }

	//------------------------------------------------------------------------
	// 入力ファイルの存在確認(後段で突然落ちないように、ここで分かりやすく知らせる)
	//------------------------------------------------------------------------
	if (check_readable("-net", fi[CIRCUIT].name) != SUCCESS) return ERROR;
	if (fi[FAULT].flag == ON && check_readable("-fault", fi[FAULT].name) != SUCCESS) return ERROR;

	//------------------------------------------------------------------------
	// default の設定
	//------------------------------------------------------------------------
	if (fi[UNTEST_MODE].flag == OFF) {
		fi[UNTEST_MODE].name = "ON";
	}
	if (fi[RAND].flag == OFF) {
		fi[RAND].name = "OFF";
	}
	if (fi[TIME].flag == OFF) {
		fi[TIME].value = 10;
	}
	if (fi[WSATH].flag == OFF) {
		fi[WSATH].value = 20;
	}
	if (fi[THREADS].flag == OFF) {
		fi[THREADS].value = 24;
	}
	if (fi[SEED].flag == OFF) {
		fi[SEED].value = (int)(time(NULL) & 0x7fffffff);	//ログに残るので、同じ結果を再現したいときは -seed で指定できる
	}

	return SUCCESS;

}
