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
#include"../../Header/Define.h"

//===================================================
//関数名:fault_judge
//機能:検出故障特定(故障辞書を読み、検出された故障を未検出故障リストから検出故障リストへ移す)
//戻り値：検出された故障数
//引数：なし
//===================================================
int fault_judge(void) {
	UNDET_FAULT_LIST *d_head;
	UNDET_FAULT_LIST *df_node;
	UNDET_FAULT_LIST *pre_node;
	UNDET_FAULT_LIST *u_node;
	UNDET_FAULT_LIST *node;
	char str[256];
	char txt_cmd[1024];
	char* p_str_type;
	int  p_type;
	char* p_str_net;
	int i = 1;
	int count = 0;
	int det_fault_num_clasp = 0;//1テストパターンでの検出故障数

	//故障辞書ファイルオープン
	snprintf(txt_cmd, sizeof(txt_cmd), "%sfsim_log.txt", folder_tmp);
	if ((fout_fsim_result = fopen(txt_cmd, "r")) == NULL) {
		printf("【ERROR】:File open fsim_log.txt(fault_judge.c)\n");
		return ERROR;
	}

	d_head = dfl;

	//----------------------------------------
	//tp0
	//STR G9
	//STF G6
	//STF G8
	//STF G16
	//STF G8_G16_B
	//----------------------------------------
	//1行目(tp0)は読み飛ばし、2行目以降の「故障タイプ 信号線名」が検出された故障

	while (fgets(str, sizeof(str), fout_fsim_result) != NULL) {
		if (ufl_num == 0)break;
		if (i != 1 && strcmp(str,"\n")!=0) { 
			p_str_type= strtok(str, " \t\n"); //STR or STF
			p_str_net = strtok(NULL, " \t\n"); //G9
			if (p_str_type == NULL || p_str_net == NULL) { i++; continue; }

			if (strcmp(p_str_type, "STR") == 0) {
				p_type = STR;
			}
			else if (strcmp(p_str_type, "STF") == 0) {
				p_type = STF;
			}
			else { i++; continue; }

			//未検出故障リストから、タイプと信号線名が一致する故障を探す
			pre_node = NULL;
			for (u_node = ufl; u_node != NULL; pre_node = u_node, u_node = u_node->next) {
				if (p_type != u_node->type || strcmp(u_node->name, p_str_net) != 0) continue;

				//検出故障ノード
				if ((df_node = (UNDET_FAULT_LIST*)calloc(1, sizeof(UNDET_FAULT_LIST))) == NULL) {
					printf("【ERROR】:No calloc detected fault node(fault_judge.c)\n");
					fclose(fout_fsim_result);
					return ERROR;
				}

				//情報コピー
				strcpy(df_node->name, u_node->name);
				df_node->type = u_node->type;
				df_node->netlist = u_node->netlist;

				//目標故障設定
				if (u_node->netlist->tf_flag == FLAG_UP) {
					count++;//目標故障が検出された場合countする
				}

				//線形リスト生成
				df_node->next = d_head;
				d_head = df_node;

				//検出、未検出故障数更新
				dfl_num++;
				det_fault_num_clasp++;

				//検出故障を未検出故障リストから削除
				ufl_remove(u_node, pre_node);

				break;
			}
		}
		i++;
	}

	//dfl 検出故障リストの更新
	dfl = d_head;

	//目標故障削除
	node = tfl;
	while (node != NULL) {
		d_head = node->next;
		free(node);
		node = d_head;
	}

	// 故障検出したらテストパターン数をインクリメント
	if (det_fault_num_clasp > 0) {
		tp_num++;
	}

	//標準出力
	standard_output();

	//ファイル操作
	fclose(fout_fsim_result);

	snprintf(txt_cmd, sizeof(txt_cmd), "%smttg.txt", folder_out);
	fout_mttg = fopen(txt_cmd, "a+");
	fprintf(fout_mttg, "%d: tfl_num:%d\ttfl_det_fault:%d\tdet_fault:%d\n", tp_num, tfl_num, count, det_fault_num_clasp);
	fclose(fout_mttg);
	
	return det_fault_num_clasp;
}
