//11
//------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include<stdlib.h>
#include<string.h>
#define _CRT_SECURE_NO_WARNINGS

//------------------------------------------------------------------------
//自作インクルード
//------------------------------------------------------------------------
#include"../Header/Define.h"

//===================================================
//関数名:main
//機能:Transition_Delay_Fault_ATPGのmain
//戻り値：int 正常終了:SUCCESS(=0) , 異常終了:ERROR(=-1)
//引数：コマンドライン引数の数, コマンドライン引数
//===================================================
int main(int argc, char *argv[]) {
	clock_t time; //タイム計測変数

	//コマンドライン解析
	if (cmdline(argc, argv) != SUCCESS) {
		printf("【ERROR】:Commandline(main.c)\n");
		exit(ERROR);
	}
	printf("【SUCCESS】:Commandline\n");

	//ネットリスト起動
	read_nl(fi[CIRCUIT].name);
	make_ppo_ppi();

	//初期化
	if (init_exturn() != SUCCESS) {
		printf("【ERROR】:init exturn(main.c)\n");
		exit(ERROR);
	}
	printf("【SUCCESS】:init exturn\n");

	//消費電力考慮の場合
	if ((strcmp(fi[LOW_POWER].name, "ON")) == 0) {
		if (check_open_pass() != SUCCESS) {
			printf("【ERROR】:open pass(main.c)\n");
			return ERROR;
		}
		printf("【SUCCESS】:open pass\n");
	}

	//タイム計測
	time = clock();

	//遷移故障MTTG開始
	if (mttg_main() != SUCCESS) {
		printf("【ERROR】:MTTG(main.c)\n");
		return ERROR;
	}
	if (fi[TARGET_LIMIT].value == 1) {
		printf("【SUCCESS】:STTG\n");
	}
	else {
		printf("【SUCCESS】:MTTG\n");
	}

	//ファイル生成
	if (file_output(time,2) != SUCCESS) {
		printf("【ERROR】:File Output(main.c)\n");
		return ERROR;
	}

	printf("\n\n----------【FINISH】Transition_Delay_Fault_ATPG----------\n");
	return SUCCESS;

}