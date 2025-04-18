//------------------------------------------------------------------------
//CN[h
//------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include<stdlib.h>
#include<string.h>

//------------------------------------------------------------------------
//©μCN[h
//------------------------------------------------------------------------
#include"../Header/Define.h"


//===================================================
//ΦΌ:unsat_dicision
//@\:SAT©UNSAT©»θ
//ίθlFint@SAT(=1),UNSAT(=0)
//ψFmode(0:eXgs\Μαν,1:ΪWΜαIπ,2:ΑοdΝΜα½])
//===================================================
int unsat_dicision(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node,int mode) {
    int i;
	char txt_cmd[300];
	char str[50000];
	char *d_str;

	//ΜζmΫ
	if ((d_str = (char*)calloc(n_pi * 2, sizeof(char))) == NULL) {
		printf("yERRORz:No calloc d_str(unsat_dicision.c)\n");
		return ERROR;
	}

	//claspΚt@CI[v
	if (mode == 0||mode==2) {
		sprintf(txt_cmd, "%sclasp_log_untest.txt",folder_out);
		if ((fin_untest_pbo_result = fopen(txt_cmd, "r")) == NULL) {
			printf("yERRORz:File open clasp_log_untest.txt(unsat_dicision.c)\n");
			exit(1);
		}
		for (i = 0; i < 4; i++) {
			fgets(str, 50000, fin_untest_pbo_result);
		}
		fclose(fin_untest_pbo_result);
	}
	else if (mode == 1) {
		sprintf(txt_cmd, "%sclasp_log_mini.txt", folder_out);
		if ((fin_mini_pbo_result = fopen(txt_cmd, "r")) == NULL) {
			printf("yERRORz:File open clasp_log_mini.txt(unsat_dicision.c)\n");
			exit(1);
		}
		for (i = 0; i < 4; i++) {
			fgets(str, 50000, fin_mini_pbo_result);
		}
		fclose(fin_mini_pbo_result);
	}

	d_str = strtok(str, "\n");
	if (strcmp(d_str, "s UNSATISFIABLE") != 0) {
		return SAT;
	}

	//eXgs\ΜανΜUNSAT
	if (mode == 0 || mode==2) {
		if (mode == 0) {//eXgs\Μαν
			//’oΜαXg©ην
			ufl_free(node, pre_node, 0);
		}
		else if (mode == 2) {//ΑοdΝ
			//’oΜαXg©ην
			ufl_free(node, pre_node, 1);
		}

		fclose(fin_untest_pbo_result);
	}

	if (mode == 1) {
		fclose(fin_mini_pbo_result);
	}

	return UNSAT;
}