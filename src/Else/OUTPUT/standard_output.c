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

//===================================================
//関数名:print_info
//機能:実行情報の出力(標準出力・ログファイル 共通)
//戻り値：なし
//引数：出力先(stdout またはログファイル)
//===================================================
void print_info(FILE* fp) {

	fprintf(fp, "//---------------------------------------------------\n");
	fprintf(fp, "//	    Transition Fault ATPG   Information\n");
	fprintf(fp, "//---------------------------------------------------\n");
	fprintf(fp, "// Target Circuit              :%s\n", fi[CIRCUIT].name);
	fprintf(fp, "// Fault Type                  :NODE\n");
	if (fi[TARGET_LIMIT].value == 1) {
		fprintf(fp, "// Test Generation             :STTG\n");
	}
	else {
		fprintf(fp, "// Test Generation             :MTTG\n");
	}
	fprintf(fp, "// Number of Total Faults      :%d\n", (int)all_num);
	fprintf(fp, "// Number of Untestable Faults :%d\n", (int)red_num);
	fprintf(fp, "// Number of High Power Faults :%d\n", (int)hpfl_num);
	fprintf(fp, "// Number of Aborted Faults    :%d\n", (int)abf_num);
	fprintf(fp, "// Number of Detected Faults   :%d\n", (int)dfl_num);
	fprintf(fp, "// Number of Undetected Faults :%d\n", (int)ufl_num);
	fprintf(fp, "// Number of Test Vectors      :%d\n", tp_num);
	fprintf(fp, "// Rate of Detected Faults     :%.2lf%%\n", (((double)dfl_num) / ((double)all_num))*100.0);
	fprintf(fp, "// Elapsed time                :%.2lfsec\n", (double)clock() / CLOCKS_PER_SEC);
	fprintf(fp, "// Time to abort solver        :%dsec\n", fi[TIME].value);
	fprintf(fp, "// Target faults limit         :%d\n", fi[TARGET_LIMIT].value);
	fprintf(fp, "// Rand Optiont                :%s\n", fi[RAND].name);
	fprintf(fp, "// Low Power Mode              :%s\n", fi[LOW_POWER].name);
	fprintf(fp, "// WSA                         :%d[％]\n", fi[WSATH].value);
	fprintf(fp, "//---------------------------------------------------\n");
}

//===================================================
//関数名:standard_output
//機能:標準出力
//戻り値：なし
//引数：なし
//===================================================
void standard_output(void) {
	print_info(stdout);
}
