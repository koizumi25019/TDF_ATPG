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
#include "../Header/Define.h"

//===================================================
//�֐���:mttg_check_wsa
//�@�\:WSA�v�Z(������d�͌̏Ⴉ�ۂ�)
//     �O�̂��߃e�X�g�������ꂽ���̂������Ċm�F
//�߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
//�����F�Ȃ�
//===================================================
int mttg_check_wsa(void) {
	char cmd[500];
	char str[20000];
	char txt_cmd[100];
	//char txt_cmd2[100];
	FILE* fout_high;
	FILE* fout_low;
	int flag = ERROR;

	//WSA���
	sprintf(cmd, "WSA.exe -sc %s -tptx tv.txt -mode 1 -rate %d -high %shigh_mttg.txt -low %slow_mttg.txt -rep %s%s_wsa_rep.txt", fi[CIRCUIT].name, fi[WSATH].value,folder_out,folder_out,folder_out, module_name);
	system(cmd);

	//���ʃt�@�C���I�[�v��(������d�̓e�X�g�p�^�[��,�����d�̓e�X�g�p�^�[��)
	sprintf(txt_cmd, "%shigh_mttg.txt", folder_out);
	if ((fout_high = (fopen(txt_cmd, "r"))) == NULL) {
		printf("�yERROR�z:File open high_mttg.txt(mttg_check_wsa.c)\n");
		return ERROR;
	}

	sprintf(txt_cmd, "%slow_mttg.txt", folder_out);
	if ((fout_low = (fopen(txt_cmd, "r"))) == NULL) {
		printf("�yERROR�z:File open low_mttg.txt(mttg_check_wsa.c)\n");
		return ERROR;
	}


	//������d�͌̏�̏ꍇ���e�X�g������������
	if ((fgets(str, n_net + 1, fout_low)) != NULL) {
		flag = SAT;
	}
	else if ((fgets(str, n_net + 1, fout_high)) != NULL) {
		flag = UNSAT;
	}

	fclose(fout_high);
	fclose(fout_low);

	return flag;

}