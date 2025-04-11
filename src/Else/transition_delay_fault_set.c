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
//関数名:Transition_Delay_Fault_Set
//機能:遷移故障全故障集合作成
//戻り値：int 正常終了:SUCCESS(=0) , 異常終了:ERROR(=-1)
//引数：なし
//===================================================
int all_transition_delay_fault_set(void) {
	int i;
	int id = 0;
	UNDET_FAULT_LIST* head, * node;

	//線形リストの初期化
	head = NULL;

	for (i = 0; i < n_net; i++) {
		switch (sort_nl[i]->type) {
		case AND:
		//STR
		//ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//情報格納
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STR;

		//全故障ID入力
	    node->node_id = id;
        id++;

		//線形リスト構築
	    node->next = head;
		//最初のnodeの手前に新しいnodeが追加されていくイメージ
		//head：現在のリストの先頭を指す
		//この操作により,新しく作成されたnodeが現在のリストの先頭ノードとして扱われる
		head = node;
		//最初NULLだったheadに現在のnode(今、値を入れていったnode)のアドレスを渡している
		ufl_num++;

		//STF
        //ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STF;

		//全故障ID入力
		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		//先ほどさしていたheadを現在のnode->nextとすることで、現在のnodeの後ろに前のnodeをリンクすることができる
		head = node;
		//headに現在のnodeのアドレスを与える
		ufl_num++;
		break;

		case NAND:
		//STR
        //ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//情報格納
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STR;

		//全故障ID入力
		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		//STF
        //ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//情報格納
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STF;

		//全故障ID入力
		node->node_id = id;
	    id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		break;

		case OR:
		//STR
        //ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//情報格納
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STR;

		//全故障ID入力
		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		//STF
        //ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//情報格納
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STF;

		//全故障ID入力
		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		break;

		case NOR:
		//STR
		//ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//情報格納
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STR;

		//全故障ID入力
		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		//STF
	    //ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//情報格納
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STF;

		//全故障ID入力
		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		break;

		case INV:
		//STR
	    //ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//情報格納
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STR;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		//STF
		//ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//情報格納
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STF;

		//全故障ID入力
		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;
		break;

		case BUF:
		//STR
		//ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//情報格納
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STR;

		//全故障ID入力
		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		//STF
		//ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//情報格納
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STF;

		//全故障ID入力
		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		break;

		case FOUT:
		//STR
		//ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STR;

		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STF;

		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		break;

		case IN:
        //STR
        //ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STR;

		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

	    node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STF;

		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		break;

		case DFF:
		//STR
		//ノード作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STR;

		node->node_id = id;
		id++;

	    //線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//故障タイプの格納
		node->type = STF;

		node->node_id = id;
		id++;

		//線形リスト構築
		node->next = head;
		head = node;
		ufl_num++;

		break;

		default:
		printf("想定していないゲートタイプです(default)");
		break;
		}
	}

	//全信号線の処理終了
	ufl = head;
	afl = head;
	node_num = id;

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
	char line[100];//ファイルの1行分を取得
	char fault_type[10];
	char net_name[50];
	//char file_name[50];
	UNDET_FAULT_LIST* head = NULL;
	UNDET_FAULT_LIST* node;
	NLIST* net_info; 

	//未検出故障集合ファイルオープン
	FILE* fp = fopen(fi[FAULT].name, "r");
	if (fp == NULL) {
		printf("【ERROR】: File open failed (%s)\n", fi[FAULT].name);
		return ERROR;
	}

	// ファイルから各行を読み取って解析
	while (fgets(line, sizeof(line), fp) != NULL) {//ファイルの1行分を取得
		if (sscanf(line, "%s %s", net_name,fault_type) != 2) {
			printf("【WARNING】: Invalid format in line: %s", line);
			continue;//現在の反復(itertion)をスキップし、次の反復に移る
		}

		// ネット名を基にソート済みネットリストから該当する情報を探す
		net_info = search_netlist(net_name); 
		if (net_info == NULL) {
			printf("【ERROR】: Net not found (%s)\n", net_name);
			continue;
		}

		// 新しい未検出故障ノードを作成
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));
		if (node == NULL) {
			printf("【ERROR】: Memory allocation failed\n");
			fclose(fp);
			return ERROR;
		}

		// 故障情報の格納 (NLIST の情報を UNDET_FAULT_LIST に反映)
		node->netlist = net_info;  // NLIST のポインタをそのまま使用可能
		strcpy(node->name, net_name);

		// 故障タイプの設定
		if (_stricmp(fault_type, "STR") == 0) {
			node->type = STR;
		}
		else if (_stricmp(fault_type, "STF") == 0) {
			node->type = STF;
		}
		else {
			printf("【WARNING】: Unknown fault type (%s)\n", fault_type);
			free(node);
			continue;
		}

		//信号線故障ID入力
		node->node_id = id;
		id++;

		// リストの先頭に追加
		node->next = head;
		head = node;
		ufl_num++;
	}

	fclose(fp);

	// ufl にリストを格納
	ufl = head;
	afl = head; 
	node_num = id;

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
