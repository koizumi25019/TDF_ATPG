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

//=========================================================================
//  関数名 : untest_cloc_pbo_size
//  機  能 : PBOのサイズを計算
//  戻り値 : なし
//  引  数 : 対象故障
//=========================================================================
PBO untest_calc_pbo_size(UNDET_FAULT_LIST* node) {

	PBO pbo;
	pbo.val = 0;
	pbo.h_constraint = 0;
	pbo.s_constraint = 0;
	pbo.lit = 0;


	//---------------------------------------------
	//制約数計算
	//---------------------------------------------
	pbo.h_constraint = untest_calc_hard_constraint(node);  //ハード制約(テスト生成モデル)
	pbo.s_constraint = untest_calc_soft_constraint();	    //ソフト制約(消費電力制約)
	pbo.h_constraint += pbo.s_constraint;

	//---------------------------------------------
	//変数計算
	//---------------------------------------------
	if (fc_ppo == 1) {
		pbo.val = suffix + fc_ppo + pbo.s_constraint - 1;
	}
	else if (fc_ppo != 1) {
		pbo.val = suffix + fc_ppo + 1 + pbo.s_constraint;
	}

	return pbo;

}


//=========================================================================
//  関数名 : untest_calc_hard_constraint
//  機  能 : 制約数計算(ハード)
//  戻り値 : なし
//  引  数 : 対象故障
//=========================================================================
int untest_calc_hard_constraint(UNDET_FAULT_LIST* node) {

	unsigned int n = 0;
	int fault_det;

	//---------------------------------------------
	//正常回路
	//---------------------------------------------
	for (int i = 0; i < n_net; i++) {
		switch (nl[i].type) {
		case AND:
			n += nl[i].n_in + 1;
			break;
		case NAND:
			n += nl[i].n_in + 1;
			break;
		case OR:
			n += nl[i].n_in + 1;
			break;
		case NOR:
			n += nl[i].n_in + 1;
			break;
		case INV:
			n += 2;
			break;
		case FOUT:
			n += 2;
			break;
		case BUF:
			n += 2;
			break;
		case EXOR:
			n += 4;
			break;
		}
	}
	n = n * 2;	//2時刻目

	//---------------------------------------------
	//故障回路
	//---------------------------------------------

	for (int i = 0; i < n_net; i++) {
		if (nl[i].fault_pass == YES && nl[i].suf_fc != n_net * 2 + 1) {
			switch (nl[i].type) {
			case AND:
				n += nl[i].n_in + 1;
				break;
			case NAND:
				n += nl[i].n_in + 1;
				break;
			case OR:
				n += nl[i].n_in + 1;
				break;
			case NOR:
				n += nl[i].n_in + 1;
				break;
			case INV:
				n += 2;
				break;
			case FOUT:
				n += 2;
				break;
			case BUF:
				n += 2;
				break;
			case EXOR:
				n += 4;
				break;
			}
		}
	}

	//---------------------------------------------
	//故障検出回路
	//---------------------------------------------
	//PPO数1つの場合【EXOR１つのみ追加】
	if (fc_ppo == 1) { n += 4; }
	//PPO数2つ以上の場合【EXOR（PPO数分）＆ ORゲート】
	else if (fc_ppo != 1) { n += fc_ppo * 4 + fc_ppo + 1; }

	//---------------------------------------------
	//1時刻目と2時刻目のFFを接続するDFF
	//---------------------------------------------
	n = n + 2 * n_dff;

	//---------------------------------------------
	//1時刻目と2時刻目のPIを接続するDFF
	//---------------------------------------------
	n = n + n_pi * 2;

	//---------------------------------------------
	//故障励起分
	//---------------------------------------------
	fault_det = 3 + 1; //STF or STR ＋det=1
	n += fault_det;

	//戻り値：制約数
	return n;
}

//=========================================================================
//  関数名 : untest_calc_soft_constraint
//  機  能 : 制約数計算(ソフト　消費電力)
//  戻り値 : なし
//  引  数 : なし
//=========================================================================
int untest_calc_soft_constraint(void) {

	int n = 0;

	for (int i = 0; i < n_net; i++) {
		if (nl[i].open_pass == NO) n++;
	}

	return n;
}