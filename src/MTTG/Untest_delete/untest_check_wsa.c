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
#include"../../Header/Define.h"

//===================================================
//関数名:untest_check_wsa
//機能:WSA計算(高消費電力故障か否か)
//     →高消費故障の場合、テスト不能故障とみなす
//戻り値：int 正常終了:SUCCESS(=0) , 異常終了:ERROR(=-1)
//引数：対象故障,対象故障の前にある故障
//===================================================
int untest_check_wsa(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node) {
	char cmd[500];
	char str[20000];
	char txt_cmd[100];
	char txt_cmd2[100];
	FILE* fout_high;
	FILE* fout_low;
	int flag = ERROR;

	//WSA解析
	sprintf(txt_cmd, "%shigh_tp.txt",folder_out);
	sprintf(txt_cmd2, "%slow_tp.txt",folder_out);
	
	sprintf(cmd, "WSA.exe -sc %s -tptx tv.txt -mode 1 -rate %d -high %s -low %s -rep %s%s_wsa_rep.txt", fi[CIRCUIT].name, fi[WSATH].value,txt_cmd,txt_cmd2,folder_out,module_name);
	system(cmd);

	//結果ファイルオープン(高消費電力テストパターン,低消費電力テストパターン)
	if ((fout_high = (fopen(txt_cmd, "r"))) == NULL) {
		printf("【ERROR】:File open high_tp.txt(untest_check_wsa.c)\n");
		return ERROR;
	}

	if ((fout_low = (fopen(txt_cmd2, "r"))) == NULL) {
		printf("【ERROR】:File open low_tp.txt(untest_check_wsa.c)\n");
		return ERROR;
	}


	//高消費電力故障の場合→テスト不能故障に追加し未検出故障からは削除
	if ((fgets(str, n_net + 1, fout_low)) != NULL) {
		flag = SAT;
	}
	else if ((fgets(str, n_net + 1, fout_high)) != NULL) {
		//未検出故障リストから削除
		ufl_free(node, pre_node, 1);
		flag = UNSAT;
	}

	fclose(fout_high);
	fclose(fout_low);
	
	return flag;

}