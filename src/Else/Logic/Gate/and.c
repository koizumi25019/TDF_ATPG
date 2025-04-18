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
#include"../../../Header/Define.h"

//=========================================================================
//  関数名 : and
//  機  能 : 論理シミュレーション(AND)
//　戻り値：int 正常終了:SUCCESS 異常終了:ERROR
//　引数：void
//=========================================================================
int and(NLIST *net, int flag) {
	int i;
	int num;
	if (flag == 1) {
		num = net->in[0]->val_1t;
		for (i = 1; i < net->n_in; i++) {
			num = num & net->in[i]->val_1t;
		}
	}
	else {
		num = net->in[0]->val_2t;
		for (i = 1; i < net->n_in; i++) {
			num = num & net->in[i]->val_2t;
		}
	}
	return num;

	return 0;
}