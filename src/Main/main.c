//11
//------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include<stdlib.h>
#include<string.h>
#define _CRT_SECURE_NO_WARNINGS

//------------------------------------------------------------------------
//����C���N���[�h
//------------------------------------------------------------------------
#include"../Header/Define.h"

//===================================================
//�֐���:main
//�@�\:Transition_Delay_Fault_ATPG��main
//�߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
//�����F�R�}���h���C�������̐�, �R�}���h���C������
//===================================================
int main(int argc, char *argv[]) {
	clock_t time; //�^�C���v���ϐ�

	//�R�}���h���C�����
	if (cmdline(argc, argv) != SUCCESS) {
		printf("�yERROR�z:Commandline(main.c)\n");
		exit(ERROR);
	}
	printf("�ySUCCESS�z:Commandline\n");

	//�l�b�g���X�g�N��
	read_nl(fi[CIRCUIT].name);
	make_ppo_ppi();

	//������
	if (init_exturn() != SUCCESS) {
		printf("�yERROR�z:init exturn(main.c)\n");
		exit(ERROR);
	}
	printf("�ySUCCESS�z:init exturn\n");

	//����d�͍l���̏ꍇ
	if ((strcmp(fi[LOW_POWER].name, "ON")) == 0) {
		if (check_open_pass() != SUCCESS) {
			printf("�yERROR�z:open pass(main.c)\n");
			return ERROR;
		}
		printf("�ySUCCESS�z:open pass\n");
	}

	//�^�C���v��
	time = clock();

	//�J�ڌ̏�MTTG�J�n
	if (mttg_main() != SUCCESS) {
		printf("�yERROR�z:MTTG(main.c)\n");
		return ERROR;
	}
	if (fi[TARGET_LIMIT].value == 1) {
		printf("�ySUCCESS�z:STTG\n");
	}
	else {
		printf("�ySUCCESS�z:MTTG\n");
	}

	//�t�@�C������
	if (file_output(time,2) != SUCCESS) {
		printf("�yERROR�z:File Output(main.c)\n");
		return ERROR;
	}

	printf("\n\n----------�yFINISH�zTransition_Delay_Fault_ATPG----------\n");
	return SUCCESS;

}