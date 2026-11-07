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
//機能：テスト不能故障・高消費電力故障・未確定故障を、未検出故障リストから専用のリストへ移す
//戻り値：void
//引数：削除したい故障，その1つ前の故障(先頭の場合は NULL)，移す理由(UFL_UNTESTABLE / UFL_HIGH_POWER / UFL_ABORT_*)
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

	//p_nodeを移し先リストの先頭に追加している
	if (mode == UFL_UNTESTABLE) {
		p_node->next = red;
		red = p_node;
		red_num++;
	}
	else if (mode == UFL_HIGH_POWER) {
		p_node->next = hpfl;
		hpfl = p_node;
		hpfl_num++;
	}
	else {	//UFL_ABORT_*
		p_node->abort_reason = mode;
		p_node->next = abf;
		abf = p_node;
		abf_num++;
	}

	//未検出故障リストから削除(free)
	ufl_remove(node, pre_node);
}
