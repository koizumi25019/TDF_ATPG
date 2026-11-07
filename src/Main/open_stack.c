//------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>

//------------------------------------------------------------------------
//自作インクルード
//------------------------------------------------------------------------
#include"../Header/Define.h"

//===================================================
//関数名:open_push
//機能:プッシュ(オープンパス)
//戻り値：なし
//引数：信号線
//===================================================
void open_push(NLIST* p) {
	int	i;

	if (Open.ptr >= Open.max_num) {
		//容量が足りなければ2倍に拡張する(固定長だと回路によっては溢れるため)
		int new_max = (Open.max_num > 0) ? Open.max_num * 2 : 1024;
		NLIST** new_stk;
		if (Open.max_num > INT_MAX / 2 || (new_stk = (NLIST**)realloc(Open.stk, sizeof(NLIST*) * new_max)) == NULL) {
			printf("stack overflow(Open_stack.c)\n");
			exit(1);
		}
		Open.stk = new_stk;
		Open.max_num = new_max;
	}
	i = Open.ptr;
	Open.stk[i] = p;
	Open.ptr++;
}

//=========================================================================
//  関数名 : open_pop
//  機  能 : ポップ(オープンパス)
//  戻り値 : 信号線
//  引  数 : なし
//=========================================================================
NLIST* open_pop(void) {
	int	i;

	if (Open.ptr < 1) {
		printf("stack underflow(Open_stack.c)\n");
		exit(1);
	}
	i = Open.ptr - 1;
	Open.ptr--;
	return Open.stk[i];
}
