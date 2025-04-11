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


////===================================================
////関数名:undet_fault_delete
////機能:未検出故障リストから故障を削除
////戻り値：int 正常終了:SUCCESS(=0) , 異常終了:ERROR(=-1)
////引数：削除したい故障
////===================================================
//int undet_fault_delete(UNDET_FAULT_LIST* node) {
//	int i, flag=FLAG_DOWN,flag_1,flag_2;
//	//UNDET_FAULT_LIST *gate_node, *gate_pre_node;
//
//	//1つ前のノードを格納する領域を初期化
//	//gate_pre_node = NULL;
//
//	//ゲートグループの方も削除
//	//ゲート内で探索開始
//	/*for (i = 0; i < gate_num; i++) {
//		if (gate_group[i] != NULL) {
//			if (strcmp(gate_group[i]->name, node->name) == 0) {
//				gate_node = gate_group[i];
//				break;
//			}
//		}
//	}*/
//	while (gate_node != NULL && flag == FLAG_DOWN) {
//		flag_1 = 0;
//		flag_2 = 0;
//		//必須割当て条件が一致するかどうか
//		for (int k = 0; k < node->input_num; k++) {
//			if (gate_node->input_1[k] == node->input_1[k]) {
//				flag_1++;
//			}
//			if (gate_node->input_2[k] == node->input_2[k]) {
//				flag_2++;
//			}
//		}
//		if ((flag_1 == node->input_num) && (flag_2 == node->input_num)) {
//
//			//ゲート内で1番最初のノード
//			if (gate_pre_node == NULL) {
//
//				//対象信号線の削除
//				gate_pre_node = gate_node->next;
//				free(gate_node);
//				gate_group[i] = gate_pre_node;
//				flag = FLAG_UP;
//				break;
//			}
//
//			//それ以外
//			else {
//				gate_pre_node->next = gate_node->next;
//				free(gate_node);
//				flag = FLAG_UP;
//				break;
//			}
//		}
//		gate_pre_node = gate_node;
//		gate_node = gate_node->next;
//	}
//
//	return SUCCESS;
//}

//===================================================
// 関数名: undet_fault_delete
// 機能: 未検出故障リストから指定された遷移故障を削除
// 戻り値：int 正常終了:SUCCESS(=0), 異常終了:ERROR(=-1)
// 引数：UNDET_FAULT_LIST* node - 削除したい故障のポインタ
//===================================================
int undet_fault_delete(UNDET_FAULT_LIST* node) {
	UNDET_FAULT_LIST* current, * prev;

	// 未検出故障リストが空、または削除対象のノードがNULLの場合
	if (ufl == NULL || node == NULL) {
		return ERROR;
	}

	current = ufl;
	prev = NULL;

	// 未検出故障リストを走査
	while (current != NULL) {
		// 故障タイプとネット名が一致するかを確認
		if (current->type == node->type && strcmp(current->name, node->name) == 0) {
			// ノードを削除
			if (prev == NULL) {
				// 削除するノードがヘッドノードの場合
				ufl = current->next;
			}
			else {
				prev->next = current->next;
			}
			// メモリを解放
			free(current);
			ufl_num--; // 未検出故障数をデクリメント
			return SUCCESS;
		}
		prev = current;
		current = current->next;
	}

	// ノードが見つからなかった場合
	return ERROR;
}



//========================================================
//関数名:ufl_free
//機能：テスト不能故障削除
//戻り値：void
//引数：削除したい故障，,テスト不能(mode=0)か高消費電力故障(mode=1)なのか
//========================================================
void ufl_free(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node,int mode) {
	UNDET_FAULT_LIST*p_node;

	p_node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

	//情報格納
	strcpy(p_node->name, node->name);
	p_node->type=node->type;
	p_node->netlist = node->netlist;

	if (mode == 0) {//テスト不能
		//p_nodeをredまたはhpflの先頭に追加している
		p_node->next = red;
		red = p_node;
	}
	else if (mode == 1) {//高消費電力
		p_node->next = hpfl;
		hpfl = p_node;
	}

	//削除(free)
	if (pre_node == NULL) {

		ufl = node->next;

		ufl_num--;

		if (mode == 0) {
			red_num++;
		}
		else if (mode == 1) {
			hpfl_num++;
		}

		free(node);

	}
	else {
		pre_node->next = node->next;

		ufl_num--;

		if (mode == 0) {
			red_num++;
		}
		else if (mode == 1) {
			hpfl_num++;
		}

		free(node);

	}

	return;
}