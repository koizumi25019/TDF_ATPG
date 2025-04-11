//------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include<stdlib.h>
#include<string.h>

//------------------------------------------------------------------------
//����C���N���[�h
//------------------------------------------------------------------------
#include"../Header/Define.h"


//===================================================
//�֐���:unsat_dicision
//�@�\:SAT��UNSAT������
//�߂�l�Fint�@SAT(=1),UNSAT(=0)
//�����Fmode(0:�e�X�g�s�\�̏�폜��,1:�ڕW�̏�I����,2:������d�͌̏ᔽ�]��)
//===================================================
int unsat_dicision(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node,int mode) {
    int i;
	char txt_cmd[300];
	char str[50000];
	char *d_str;

	//�̈�m��
	if ((d_str = (char*)calloc(n_pi * 2, sizeof(char))) == NULL) {
		printf("�yERROR�z:No calloc d_str(unsat_dicision.c)\n");
		return ERROR;
	}

	//clasp���ʃt�@�C���I�[�v��
	if (mode == 0||mode==2) {
		sprintf(txt_cmd, "%sclasp_log_untest.txt",folder_out);
		if ((fin_untest_pbo_result = fopen(txt_cmd, "r")) == NULL) {
			printf("�yERROR�z:File open clasp_log_untest.txt(unsat_dicision.c)\n");
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
			printf("�yERROR�z:File open clasp_log_mini.txt(unsat_dicision.c)\n");
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

	//�e�X�g�s�\�̏�폜����UNSAT
	if (mode == 0 || mode==2) {
		if (mode == 0) {//�e�X�g�s�\�̏�폜
			//�����o�̏჊�X�g����폜
			ufl_free(node, pre_node, 0);
		}
		else if (mode == 2) {//������d��
			//�����o�̏჊�X�g����폜
			ufl_free(node, pre_node, 1);
		}

		fclose(fin_untest_pbo_result);
	}

	if (mode == 1) {
		fclose(fin_mini_pbo_result);
	}

	return UNSAT;
}