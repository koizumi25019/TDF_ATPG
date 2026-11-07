//------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include<stdlib.h>
#include<string.h>

//------------------------------------------------------------------------
//自作インクルード
//------------------------------------------------------------------------
#include"../../Header/Define.h"

//===================================================
//関数名:untest_delete_main
//機能:冗長故障削除のmain
//戻り値：int 正常終了:SUCCESS(=0) , 異常終了:ERROR(=-1)
//引数：なし
//===================================================
int untest_delete_main(void) {
	int i = 0;
	int count_a = 0;
	int sat_flag;
	int wsa_flag;
	char cmd[2048];
	char txt_cmd[1024];
	UNDET_FAULT_LIST* node;
	UNDET_FAULT_LIST* pre_node = NULL;

	node = ufl;

	//単一故障テストパターンファイル
	snprintf(txt_cmd, sizeof(txt_cmd), "%suntest_delete_tp.txt", folder_out);
	if ((fout_untest_tp = (fopen(txt_cmd, "w"))) == NULL) {
		printf("【ERROR】:File open untest_delete_tp.txt(untest_delete_main.c)\n");
		return ERROR;
	}

	//標準出力
	printf("//===========================================\n");
	printf("//Untestable Fault Delete start\n");
	printf("//===========================================\n");

	//progress bar
	printf("|----+----|----+----|----+----|----+----|----+----|\n");

	while (node != NULL) {

		//progress bar
		if (((int)i++ % (int)(((ufl_num + red_num + hpfl_num) * 2 / 100) + 1)) == 0) {
			printf("*");
			count_a++;
		}

		//故障範囲特定
		suffix = 2 * n_net + 1;
		specific_fault_range(node->netlist);

		//故障が影響を与えるPPOが1つも無い(外部出力(PO)にしか届かない等)場合、検出回路が
		//「検出=1 かつ 検出=0」になり必ず UNSAT になる。clasp を呼ぶまでもなくテスト不能とする
		if (fc_ppo == 0) {
			ufl_free(node, pre_node, UFL_UNTESTABLE);
			if (pre_node == NULL)node = ufl;
			else node = pre_node->next;
			continue;
		}

		//PBOファイル
		snprintf(txt_cmd, sizeof(txt_cmd), "%suntest_pbo.txt", folder_tmp);
		if ((fout_untest_pbo = fopen(txt_cmd, "w")) == NULL) {
			printf("【ERROR】:File open untest_pbo.txt(untest_delete_main.c)\n");
			exit(1);
		}

		//PBOサイズ記述
		untest_pbo_generate_min(fout_untest_pbo, node);

		//正常回路の記述
		pbo_generate_normal_c(fout_untest_pbo);

		//故障回路の記述
		pbo_generate_fault_c(fout_untest_pbo, node);

		//故障検出回路の記述
		pbo_generate_fdetect_c(fout_untest_pbo, node);


		//PBOファイルクローズ
		fclose(fout_untest_pbo);

		//========================================================================
		//	clasp ⇒ テストベクトル生成
		//========================================================================
		//Clasp
		snprintf(txt_cmd, sizeof(txt_cmd), "%sclasp_log_untest.txt", folder_tmp);
		snprintf(cmd, sizeof(cmd), "clasp.exe %suntest_pbo.txt --time-limit %d >%s", folder_tmp, fi[TIME].value, txt_cmd);
		system(cmd);

		//UNSAT(テスト不能)・判定できなかった(未確定)場合、未検出故障集合から外れる(node は解放済み)
		if ((sat_flag = (unsat_dicision(node, pre_node, 0))) != SAT) {
			if (pre_node == NULL)node = ufl;
			else node = pre_node->next;
			continue;
		}

		if (sat_flag == SAT && ((strcmp(fi[LOW_POWER].name, "OFF")) == 0)) {
			pre_node = node;
			node = node->next;
		}

		if (sat_flag == SAT && ((strcmp(fi[LOW_POWER].name, "ON")) == 0)) {

			//消費電力制約を追加

			//PBOファイル
			snprintf(txt_cmd, sizeof(txt_cmd), "%suntest_pbo.txt", folder_tmp);
			if ((fout_untest_pbo = fopen(txt_cmd, "a+")) == NULL) {
				printf("【ERROR】:File open untest_pbo.txt(untest_delete_main.c)\n");
				exit(1);
			}

			//消費電力制約
			pbo_generate_power(fout_untest_pbo);

			//PBOファイルクローズ
			fclose(fout_untest_pbo);

			//========================================================================
			//	clasp ⇒ テストベクトル生成
			//========================================================================
			//Clasp
			snprintf(txt_cmd, sizeof(txt_cmd), "%sclasp_log_untest.txt", folder_tmp);
			snprintf(cmd, sizeof(cmd), "clasp.exe %suntest_pbo.txt --time-limit %d >%s", folder_tmp, fi[TIME].value, txt_cmd);
			system(cmd);

			//UNSATだった場合、未検出故障集合から削除→高消費電力故障
			if ((sat_flag = (unsat_dicision(node, pre_node, 2))) != SAT) {
				//UNSAT(高消費電力故障)・判定できなかった(未確定故障)場合は ufl から外れている(node は解放済み)
				if (pre_node == NULL)node = ufl;
				else node = pre_node->next;
				continue;
			}
			else {
				//テストベクトル生成
				if (generate_test_vector(txt_cmd) != SUCCESS) {
					printf("【ERROR】:Generate Test Vector(untest_delete_main.c)\n");
					exit(1);
				}

				//消費電力解析
				if ((wsa_flag = untest_check_wsa(node, pre_node)) == SAT) {
					//テスト集合に追加
					make_test_set(fout_untest_tp);
					//SATの場合
					pre_node = node;
					node = node->next;
				}
				else if (wsa_flag == UNSAT) {
					//高消費電力だったテスト: untest_check_wsa が既にこの故障を高消費電力故障として
					//ufl から外している(node は解放済み)ので、次の故障を取り直して続ける
					if (pre_node == NULL)node = ufl;
					else node = pre_node->next;
					continue;
				}
				else {
					printf("【ERROR】:Untest Check WSA(untest_delete_main.c)\n");
					exit(1);
				}
			}
		}
	}

	printf("\n");
	fclose(fout_untest_tp);

	return SUCCESS;
}