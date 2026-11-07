//------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

//------------------------------------------------------------------------
//自作インクルード
//------------------------------------------------------------------------
#include"../Header/Define.h"

//===================================================
//関数名:mttg_main
//機能:遷移故障のMTTGのメイン
//戻り値：int 正常終了:SUCCESS(=0) , 異常終了:ERROR(=-1)
//引数：なし
//===================================================
//目標故障リスト(tfl)の解放(fault_judge を通らない場合用)
static void free_target_faults(void) {
	UNDET_FAULT_LIST* next;
	while (tfl != NULL) {
		next = tfl->next;
		free(tfl);
		tfl = next;
	}
}

int mttg_main(void) {
	char txt_cmd[1024];
	char cmd[2048];
	int unsat_flag = OFF;
	UNDET_FAULT_LIST *head;
	int det_fault_clasp_num;
	int clasp_st = SAT;		//直近の clasp の判定結果(SAT / UNSAT / ERROR=判定できなかった)

	//テストパターンファイルオープン
	snprintf(txt_cmd, sizeof(txt_cmd), "%stp_set.txt", folder_out);
	if ((fout_tp = (fopen(txt_cmd, "w"))) == NULL) {
		printf("【ERROR】:File open tp_set.txt(mttg_main.c)\n");
		return ERROR;
	}
	//PINファイル生成
	if (pin_make() != SUCCESS) {
		printf("【ERROR】:pin make(mttg_main.c)\n");
		return ERROR;
	}

	//レベライズ
	if (levelize() != SUCCESS) {
		printf("【ERROR】:levelize(mttg_main.c)\n");
		return ERROR;
	}

	//未検出故障リストの作成
	if(fi[FAULT].flag==ON){
		if (input_transition_delay_fault_set() != SUCCESS) {
			printf("【ERROR】:Transition_Delay_Fault_Set(mttg_main.c)\n");
			return ERROR;
		}
	}
	else {
		if (all_transition_delay_fault_set() != SUCCESS) {
			printf("【ERROR】:Transition_Delay_Fault_Set(mttg_main.c)\n");
			return ERROR;
		}
	}
	//全故障数の格納
	all_num = ufl_num;

	//テスト不能故障削除モード＝ON
	if (strcmp(fi[UNTEST_MODE].name, "ON") == 0) {
		////未検出故障リストの作成
		//テスト不能故障関連ファイル出力
		file_output(0, 1);
		//テスト不能故障削除
		if (untest_delete_main() != SUCCESS) {
			printf("【ERROR】:Untest Fault Delete(mttg_main.c)\n");
			return ERROR;
		}
		//テスト不能故障関連ファイル出力
		file_output(0, 0);
	}

	//コンソールアプリケーションの初期化
	system("cls");

	//標準出力
	standard_output();

	//テスト生成
	while (ufl_num != 0) {
		//PBOファイル
		snprintf(txt_cmd, sizeof(txt_cmd), "%smttg_pbo.txt", folder_tmp);
		if ((fout_mttg_pbo = fopen(txt_cmd, "w")) == NULL) {
			printf("【ERROR】:File open mttg_pbo.txt(mttg_main.c)\n");
			exit(1);
		}

		//目標故障選択
		if (fault_select(unsat_flag) != SUCCESS) {
			printf("【ERROR】:Fault Select(mttg_main.c)\n");
			return ERROR;
		}
		file_output(0, 3);

		//PBOサイズ記述
		mttg_pbo_generate_min(fout_mttg_pbo);

		//正常回路の記述
		pbo_generate_normal_c(fout_mttg_pbo);

		suffix = n_net * 2 + 1;
		head = tfl;
		while (head != NULL) {
			//故障範囲の特定
			specific_fault_range(head->netlist);
			//故障回路の記述
			pbo_generate_fault_c(fout_mttg_pbo, head);
			//故障検出回路の記述
			pbo_generate_fdetect_circuit(fout_mttg_pbo, head);
			//次の故障
			head = head->next;
		}
		printf("\n");
		
		//故障励起(1個目はハード制約、2個目以降はソフト制約)
		for (head = tfl; head != NULL; head = head->next) {
			pbo_generate_fault_ex(fout_mttg_pbo, head, (head == tfl) ? 0 : 1);
		}

		//低消費電力制約
		if (strcmp(fi[LOW_POWER].name, "ON") == 0) {
			pbo_generate_power(fout_mttg_pbo);
		}

		fclose(fout_mttg_pbo);

		//========================================================================
		//	clasp ⇒ テストベクトル生成
		//========================================================================
		//Clasp
		snprintf(cmd, sizeof(cmd), "clasp.exe  %smttg_pbo.txt --parallel-mode 24 --time-limit %d >%sclasp_log_mttg.txt", folder_tmp, fi[TIME].value, folder_tmp);
		system(cmd);
		//解が得られたか確認(UNSAT・打ち切りで解なしの場合は、この目標故障ではテストを作れなかった)
		snprintf(txt_cmd, sizeof(txt_cmd), "%sclasp_log_mttg.txt", folder_tmp);
		clasp_st = read_clasp_status(txt_cmd);
		if (clasp_st != SAT) {
			free_target_faults();
			det_fault_clasp_num = 0;
			goto no_test_generated;
		}
		//テストベクトル生成(tv.txt は中間生成フォルダーへ移される)
		if (generate_test_vector(txt_cmd) != SUCCESS) {
			printf("【ERROR】:Generate Test Vector(mttg_main.c)\n");
			return ERROR;
		}

		//低消費テストパターン確認(消費電力指向時のみ)
		if ((mttg_check_wsa() != SAT) && (strcmp(fi[LOW_POWER].name, "ON") == 0)) {
			printf("【ERROR】:MTTG TEST NG PATTERN(mttg_main.c)\n");
			return ERROR;
		}

		//========================================================================
		//	故障シミュレーション
		//========================================================================
		snprintf(cmd, sizeof(cmd), "XID2.exe -c %s -tx %s -pin %spin.txt -fm TDF -fsim YES -dic %sfsim_log.txt -det %sxid_det.txt -undet %sxid_undet.txt", fi[CIRCUIT].name, tv_path, folder_out, folder_tmp, folder_tmp, folder_tmp);
		system(cmd);

		//コンソールアプリケーションの初期化
		system("cls");

		//検出故障特定&検出故障数を未検出故障から削除
		if ((det_fault_clasp_num=fault_judge()) == ERROR) {
			printf("【ERROR】:Fault Judge(mttg_main.c)\n");
			return ERROR;
		}

		//故障を1つでも検出したテストだけをテスト集合に追加(検出0のテストは不要)
		if (det_fault_clasp_num > 0) {
			make_test_set(fout_tp);
		}
		
no_test_generated:
		//例外終了条件
		if (det_fault_clasp_num == 0) {
			if (unsat_flag == ON) {
				//1つの故障だけを狙っても検出できなかった → この故障は諦めて ufl から外す(同じ故障を永久に選び続けないため)
				//電力制約なしでUNSATならテスト不能と言える。それ以外(UNKNOWNなど・電力制約ありのUNSAT)は未確定故障にする
				printf("【WARNING】: %s could not be detected. Skip.\n", ufl->name);
				ufl_free(ufl, NULL, (clasp_st == UNSAT && strcmp(fi[LOW_POWER].name, "OFF") == 0) ? UFL_UNTESTABLE : UFL_ABORT_MTTG);
			}
			else unsat_flag = ON;
		}
		else unsat_flag = OFF;

	}

	fclose(fout_tp);

	return SUCCESS;
}
