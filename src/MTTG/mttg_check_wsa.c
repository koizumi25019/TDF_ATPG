//------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//------------------------------------------------------------------------
//自作インクルード
//------------------------------------------------------------------------
#include "../Header/Define.h"

//===================================================
//関数名:check_wsa
//機能:WSA計算(テストパターンが低消費電力か高消費電力か)
//     tv.txt を WSA.exe で解析し、結果ファイルの中身で判定する
//     (テスト不能故障削除・MTTG 共通)
//戻り値：int 低消費電力:SAT , 高消費電力:UNSAT , 判定できない:ERROR
//引数：高消費電力テストパターンの結果ファイル名, 低消費電力テストパターンの結果ファイル名(どちらも中間生成フォルダー内)
//===================================================
int check_wsa(const char* high_name, const char* low_name) {
	char cmd[2048];
	char high_path[1024];
	char low_path[1024];
	FILE* fp_high;
	FILE* fp_low;
	int flag = ERROR;

	snprintf(high_path, sizeof(high_path), "%s%s", folder_tmp, high_name);
	snprintf(low_path, sizeof(low_path), "%s%s", folder_tmp, low_name);

	//WSA解析
	snprintf(cmd, sizeof(cmd), "WSA.exe -sc %s -tptx %s -mode 1 -rate %d -high %s -low %s -rep %s%s_wsa_rep.txt", fi[CIRCUIT].name, tv_path, fi[WSATH].value, high_path, low_path, folder_tmp, module_name);
	system(cmd);

	//結果ファイルオープン(高消費電力テストパターン,低消費電力テストパターン)
	if ((fp_high = fopen(high_path, "r")) == NULL) {
		printf("【ERROR】:File open %s(mttg_check_wsa.c)\n", high_name);
		return ERROR;
	}
	if ((fp_low = fopen(low_path, "r")) == NULL) {
		printf("【ERROR】:File open %s(mttg_check_wsa.c)\n", low_name);
		fclose(fp_high);
		return ERROR;
	}

	//結果ファイルに1文字でもあれば、そちらに分類されたということ(低消費を先に判定)
	if (fgetc(fp_low) != EOF) {
		flag = SAT;
	}
	else if (fgetc(fp_high) != EOF) {
		flag = UNSAT;
	}

	fclose(fp_high);
	fclose(fp_low);

	return flag;
}

//===================================================
//関数名:mttg_check_wsa
//機能:WSA計算(高消費電力故障か否か)
//     念のためテスト生成されたものが低消費か再確認
//戻り値：int 低消費電力:SAT , 高消費電力:UNSAT , 判定できない:ERROR
//引数：なし
//===================================================
int mttg_check_wsa(void) {
	return check_wsa("high_mttg.txt", "low_mttg.txt");
}
