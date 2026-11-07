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
#include"../../Header/Define.h"

//===================================================
//関数名:level_push
//機能:プッシュ(レベライズ)
//戻り値：なし
//引数：信号線
//===================================================
void level_push(NLIST* p) {
	int	i;

	if (Level.ptr >= Level.max_num) {
		//容量が足りなければ2倍に拡張する(固定長だと回路によっては溢れるため)
		int new_max = (Level.max_num > 0) ? Level.max_num * 2 : 1024;
		NLIST** new_stk;
		if (Level.max_num > INT_MAX / 2 || (new_stk = (NLIST**)realloc(Level.stk, sizeof(NLIST*) * new_max)) == NULL) {
			printf("stack overflow(Level_stack.c)\n");
			exit(1);
		}
		Level.stk = new_stk;
		Level.max_num = new_max;
	}
	i = Level.ptr;
	Level.stk[i] = p;
	Level.ptr++;
}

//=========================================================================
//  関数名 : level_pop
//  機  能 : ポップ(レベライズ)
//  戻り値 : 信号線
//  引  数 : なし
//=========================================================================
NLIST* level_pop(void) {
	int	i;

	if (Level.ptr < 1) {
		printf("stack underflow(Level_stack.c)\n");
		exit(1);
	}
	i = Level.ptr - 1;
	Level.ptr--;
	return Level.stk[i];
}