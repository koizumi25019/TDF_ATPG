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
#include"../Header/Define.h"


//========================================================
//関数名:ufl_remove
//機能：未検出故障リストから故障を取り除く(メモリも解放)
//戻り値：void
//引数：削除したい故障，その1つ前の故障(先頭の場合は NULL)
//========================================================
void ufl_remove(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node) {

	if (pre_node == NULL) {
		ufl = node->next;
	}
	else {
		pre_node->next = node->next;
	}

	ufl_num--;

	free(node);
}

//========================================================
//関数名:ufl_free
//機能：テスト不能故障 または 高消費電力故障 を、未検出故障リストから専用のリストへ移す
//戻り値：void
//引数：削除したい故障，その1つ前の故障(先頭の場合は NULL)，テスト不能(mode=0)か高消費電力故障(mode=1)なのか
//========================================================
void ufl_free(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node,int mode) {
	UNDET_FAULT_LIST*p_node;

	p_node = (UNDET_FAULT_LIST*)calloc(1, sizeof(UNDET_FAULT_LIST));
	if (p_node == NULL) {
		printf("【ERROR】:No calloc fault node(undet_fault_delete.c)\n");
		exit(1);
	}

	//情報格納
	strcpy(p_node->name, node->name);
	p_node->type=node->type;
	p_node->netlist = node->netlist;

	//p_nodeをredまたはhpflの先頭に追加している
	if (mode == 0) {//テスト不能
		p_node->next = red;
		red = p_node;
		red_num++;
	}
	else {//高消費電力(mode=1)
		p_node->next = hpfl;
		hpfl = p_node;
		hpfl_num++;
	}

	//未検出故障リストから削除(free)
	ufl_remove(node, pre_node);
}
