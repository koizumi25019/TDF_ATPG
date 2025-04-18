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
//  関数名 : or
//  機  能 : 論理シミュレーション(OR)
//　戻り値：int 正常終了:SUCCESS 異常終了:ERROR
//　引数：void
//=========================================================================
int or(NLIST *net, int flag) {
	int i;
	int num;
	if (flag == 1) {
		for (i = 0, num = 0; i < net->n_in; i++) {
			num = num | net->in[i]->val_1t;
		}
	}
	else {
		for (i = 0, num = 0; i < net->n_in; i++) {
			num = num | net->in[i]->val_2t;
		}
	}
	return num;
}