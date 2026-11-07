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


//===================================================
//関数名:read_clasp_status
//機能:clasp の結果ファイルから、結果行("s ..." で始まる行)を探して判定する
//     (固定の行番号を読むのではなく、行頭が "s " の行を探すので、出力行数が変わっても大丈夫)
//戻り値：SAT(解あり) / UNSAT(解なし) / ERROR(打ち切り("s UNKNOWN")・結果行が無い・ファイルが読めない)
//引数：結果ファイル名
//===================================================
int read_clasp_status(const char* path) {
	char line[1024];
	int result = ERROR;
	FILE* fp = fopen(path, "r");

	if (fp == NULL) {
		printf("【ERROR】:File open %s(unsat_dicision.c)\n", path);
		return ERROR;
	}
	//clasp は起動すれば必ず "c clasp version ..." などを出力する。ログが空なら clasp が実行できていない
	if (fgetc(fp) == EOF) {
		printf("【ERROR】:%s is empty. Is clasp.exe available? (unsat_dicision.c)\n", path);
		fclose(fp);
		exit(1);
	}
	rewind(fp);
	while (fgets(line, sizeof(line), fp) != NULL) {
		if (strncmp(line, "s ", 2) != 0) continue;
		if (strncmp(line, "s UNSATISFIABLE", 15) == 0) result = UNSAT;
		else if (strncmp(line, "s SATISFIABLE", 13) == 0 || strncmp(line, "s OPTIMUM FOUND", 15) == 0) result = SAT;
		break;	//最初の結果行だけ見る(それ以外 "s UNKNOWN" など は ERROR のまま)
	}
	fclose(fp);

	return result;
}

//===================================================
//関数名:unsat_dicision
//機能:SATかUNSATか判定し、UNSATなら未検出故障リストから外す
//     UNSAT → テスト不能故障(mode=0) / 高消費電力故障(mode=2) として ufl から外す
//     判定できない(UNKNOWN・打ち切り) → 未確定故障として ufl から外す(削除はしても、UNSATとは区別して記録する)
//戻り値：int　SAT(=1),UNSAT(=0),ERROR(=-1:判定できなかった)。UNSAT・ERROR のときは node は既に解放済み
//引数：mode(0:テスト不能故障削除時,2:高消費電力故障反転時)
//===================================================
int unsat_dicision(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node,int mode) {
	char txt_cmd[1024];
	int result;

	snprintf(txt_cmd, sizeof(txt_cmd), "%sclasp_log_untest.txt", folder_tmp);
	result = read_clasp_status(txt_cmd);

	if (result == UNSAT) {
		ufl_free(node, pre_node, (mode == 2) ? UFL_HIGH_POWER : UFL_UNTESTABLE);
	}
	else if (result == ERROR) {
		ufl_free(node, pre_node, (mode == 2) ? UFL_ABORT_POWER : UFL_ABORT_NORMAL);
	}

	return result;
}
