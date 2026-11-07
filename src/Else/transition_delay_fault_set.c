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
#include"../Header/Define.h"

//===================================================
//関数名:add_fault
//機能:故障を1つ作り、リストの先頭に追加する(未検出故障数も更新)
//戻り値：int 正常終了:SUCCESS(=0) , 異常終了:ERROR(=-1)
//引数：リストの先頭(更新される), 対象信号線, 故障タイプ(STR/STF), 次に割り当てる故障ID(更新される)
//===================================================
static int add_fault(UNDET_FAULT_LIST** head, NLIST* net, int type, int* id) {
	UNDET_FAULT_LIST* node;

	//calloc なので、設定しない項目(tfl_* など)も0で初期化される
	if ((node = (UNDET_FAULT_LIST*)calloc(1, sizeof(UNDET_FAULT_LIST))) == NULL) {
		printf("【ERROR】: Memory allocation failed\n");
		return ERROR;
	}

	snprintf(node->name, sizeof(node->name), "%s", net->name);
	node->netlist = net;
	node->type = type;
	node->node_id = *id;	//全故障ID
	(*id)++;

	//線形リスト構築(先頭に追加)
	node->next = *head;
	*head = node;
	ufl_num++;

	return SUCCESS;
}

//===================================================
//関数名:Transition_Delay_Fault_Set
//機能:遷移故障全故障集合作成(論理ゲート・FF・外部入力の出力信号線に STR と STF を作る)
//戻り値：int 正常終了:SUCCESS(=0) , 異常終了:ERROR(=-1)
//引数：なし
//===================================================
int all_transition_delay_fault_set(void) {
	int i;
	int id = 0;
	UNDET_FAULT_LIST* head = NULL;

	for (i = 0; i < n_net; i++) {
		switch (sort_nl[i]->type) {
		case AND:
		case NAND:
		case OR:
		case NOR:
		case INV:
		case BUF:
		case FOUT:
		case IN:
		case DFF:
			if (add_fault(&head, sort_nl[i], STR, &id) != SUCCESS) return ERROR;
			if (add_fault(&head, sort_nl[i], STF, &id) != SUCCESS) return ERROR;
			break;

		default:
			printf("想定していないゲートタイプです(default)");
			break;
		}
	}

	//全信号線の処理終了
	ufl = head;
	afl = head;

	return SUCCESS;
}


//===================================================
// 関数名:Input_Transition_Delay_Fault_Set
// 機能:遷移故障の未検出故障集合を構造体に格納
// 戻り値：int 正常終了:SUCCESS(=0) , 異常終了:ERROR(=-1)
// 引数：なし
//===================================================

int input_transition_delay_fault_set(void) {

	int id = 0;
	int type;
	char line[1024];//ファイルの1行分を取得
	char fault_type[16];
	char net_name[256];
	UNDET_FAULT_LIST* head = NULL;
	NLIST* net_info;

	//未検出故障集合ファイルオープン
	FILE* fp = fopen(fi[FAULT].name, "r");
	if (fp == NULL) {
		printf("【ERROR】: File open failed (%s)\n", fi[FAULT].name);
		return ERROR;
	}

	// ファイルから各行を読み取って解析
	while (fgets(line, sizeof(line), fp) != NULL) {//ファイルの1行分を取得
		//(%255s などは、信号線名が長くても領域を超えて書き込まないための幅指定)
		if (sscanf(line, "%255s %15s", net_name, fault_type) != 2) {
			printf("【WARNING】: Invalid format in line: %s", line);
			continue;//現在の反復(itertion)をスキップし、次の反復に移る
		}

		// ネット名を基にソート済みネットリストから該当する情報を探す
		net_info = search_netlist(net_name);
		if (net_info == NULL) {
			printf("【ERROR】: Net not found (%s)\n", net_name);
			continue;
		}

		// 故障タイプの設定
		if (_stricmp(fault_type, "STR") == 0) {
			type = STR;
		}
		else if (_stricmp(fault_type, "STF") == 0) {
			type = STF;
		}
		else {
			printf("【WARNING】: Unknown fault type (%s)\n", fault_type);
			continue;
		}

		// 新しい未検出故障ノードを作成してリストの先頭に追加
		if (add_fault(&head, net_info, type, &id) != SUCCESS) {
			fclose(fp);
			return ERROR;
		}
	}

	fclose(fp);

	// ufl にリストを格納
	ufl = head;
	afl = head;

	printf("【INFO】: Transition fault set created successfully.\n");
	return SUCCESS;
}

//===================================================
// 関数名:search_netlist
// 機能:ネット名からソートネットリストを探索し、該当するノードの情報を返す
// 戻り値：UNDET_FAULT_LIST* 該当するノード, 見つからない場合はNULL
// 引数：char* name - ネット名
//===================================================
NLIST* search_netlist(char* name) {
	for (int i = 0; i < n_net; i++) {
		if (strcmp(sort_nl[i]->name, name) == 0) {
			return sort_nl[i];
		}
	}
	return NULL;
}
