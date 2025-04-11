//------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//------------------------------------------------------------------------
//����C���N���[�h
//------------------------------------------------------------------------
#include"../../Header/Define.h"

//===================================================
//�֐���:untest_check_wsa
//�@�\:WSA�v�Z(������d�͌̏Ⴉ�ۂ�)
//     ��������̏�̏ꍇ�A�e�X�g�s�\�̏�Ƃ݂Ȃ�
//�߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
//�����F�Ώی̏�,�Ώی̏�̑O�ɂ���̏�
//===================================================
int untest_check_wsa(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node) {
	char cmd[500];
	char str[20000];
	char txt_cmd[100];
	char txt_cmd2[100];
	FILE* fout_high;
	FILE* fout_low;
	int flag = ERROR;

	//WSA���
	sprintf(txt_cmd, "%shigh_tp.txt",folder_out);
	sprintf(txt_cmd2, "%slow_tp.txt",folder_out);
	
	sprintf(cmd, "WSA.exe -sc %s -tptx tv.txt -mode 1 -rate %d -high %s -low %s -rep %s%s_wsa_rep.txt", fi[CIRCUIT].name, fi[WSATH].value,txt_cmd,txt_cmd2,folder_out,module_name);
	system(cmd);

	//���ʃt�@�C���I�[�v��(������d�̓e�X�g�p�^�[��,�����d�̓e�X�g�p�^�[��)
	if ((fout_high = (fopen(txt_cmd, "r"))) == NULL) {
		printf("�yERROR�z:File open high_tp.txt(untest_check_wsa.c)\n");
		return ERROR;
	}

	if ((fout_low = (fopen(txt_cmd2, "r"))) == NULL) {
		printf("�yERROR�z:File open low_tp.txt(untest_check_wsa.c)\n");
		return ERROR;
	}


	//������d�͌̏�̏ꍇ���e�X�g�s�\�̏�ɒǉ��������o�̏Ⴉ��͍폜
	if ((fgets(str, n_net + 1, fout_low)) != NULL) {
		flag = SAT;
	}
	else if ((fgets(str, n_net + 1, fout_high)) != NULL) {
		//�����o�̏჊�X�g����폜
		ufl_free(node, pre_node, 1);
		flag = UNSAT;
	}

	fclose(fout_high);
	fclose(fout_low);
	
	return flag;

}