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
	int wsa_st;				//WSA の判定結果(SAT=低消費電力 / UNSAT=高消費電力 / ERROR=判定できなかった)
	int wsa_high;			//今回のテストが高消費電力だったか
	int xid_missing = 0;	//故障辞書が作られなかった連続回数
	char dic[1024];			//故障辞書のファイル名
	FILE* fp_dic;

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
		wsa_high = 0;

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
		snprintf(cmd, sizeof(cmd), "clasp.exe  %smttg_pbo.txt --parallel-mode %d --time-limit %d >%sclasp_log_mttg.txt", folder_tmp, fi[THREADS].value, fi[TIME].value, folder_tmp);
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

		//低消費テストパターン確認(消費電力指向時のみ。OFFのときは WSA.exe を実行しない)
		if (strcmp(fi[LOW_POWER].name, "ON") == 0) {
			wsa_st = mttg_check_wsa();
			if (wsa_st == ERROR) {
				printf("【ERROR】:WSA result could not be read(mttg_main.c)\n");
				return ERROR;
			}
			if (wsa_st == UNSAT) {
				//PBOの電力制約を付けて作ったのに WSA.exe が高消費電力と判定したテストは使わない
				//(単一目標でも同じなら、その故障は高消費電力故障として外す)
				printf("【WARNING】: A high power test pattern was generated. Discard.\n");
				free_target_faults();
				det_fault_clasp_num = 0;
				wsa_high = 1;
				goto no_test_generated;
			}
		}

		//========================================================================
		//	故障シミュレーション
		//========================================================================
		//前回の故障辞書が残っていると、XID2.exe が失敗したときに古い辞書を読んでしまうので先に消す
		snprintf(dic, sizeof(dic), "%sfsim_log.txt", folder_tmp);
		remove(dic);
		snprintf(cmd, sizeof(cmd), "XID2.exe -c %s -tx %s -pin %spin.txt -fm TDF -fsim YES -dic %sfsim_log.txt -det %sxid_det.txt -undet %sxid_undet.txt", fi[CIRCUIT].name, tv_path, folder_out, folder_tmp, folder_tmp, folder_tmp);
		system(cmd);

		//コンソールアプリケーションの初期化
		system("cls");

		//故障辞書が作られたか確認。作られなければ検出0として扱うが、続くなら XID2.exe の異常とみなして止める
		if ((fp_dic = fopen(dic, "r")) == NULL) {
			xid_missing++;
			printf("【WARNING】: Fault dictionary was not generated by XID2.exe (%d)\n", xid_missing);
			if (xid_missing >= 3) {
				printf("【ERROR】:XID2.exe seems not working(mttg_main.c)\n");
				return ERROR;
			}
			free_target_faults();
			det_fault_clasp_num = 0;
			goto no_test_generated;
		}
		fclose(fp_dic);
		xid_missing = 0;

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
				ufl_free(ufl, NULL, wsa_high ? UFL_HIGH_POWER :
					(clasp_st == UNSAT && strcmp(fi[LOW_POWER].name, "OFF") == 0) ? UFL_UNTESTABLE : UFL_ABORT_MTTG);
			}
			else unsat_flag = ON;
		}
		else unsat_flag = OFF;

	}

	fclose(fout_tp);

	return SUCCESS;
}
