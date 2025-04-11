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
#include"../../Header/Define.h"

//===================================================
//�֐���:pin_make
//�@�\:PIN�t�@�C���쐬
//�߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
//�����F�Ȃ�
//===================================================
int pin_make(void) {
	int i;
	char txt_cmd[300];

	//���̓s���t�@�C���I�[�v��
	sprintf(txt_cmd, "%spin_input.txt", folder_out);
	if ((fout_pin_input = (fopen(txt_cmd, "w"))) == NULL) {
		printf("�yERROR�z:File open pin_input.txt(pin_make.c)\n");
		return ERROR;
	}
	//���̓s���t�@�C���I�[�v��(�̏�V�~�����[�V����)
	sprintf(txt_cmd, "%spin.txt", folder_out);
	if ((fout_pin = (fopen(txt_cmd, "w"))) == NULL) {
		printf("�yERROR�z:File open pin.txt(pin_make.c)\n");
		return ERROR;
	}

	//�O������
	for (i = 0; i < n_pi; i++) {
		fprintf(fout_pin_input, "%s\n", pi[i]->name);
		fprintf(fout_pin, "%s\n", pi[i]->name);
	}
	//�^���O������
	for (i = 0; i < n_dff; i++) {
		fprintf(fout_pin_input, "%s\n", dff[i]->name); 
		fprintf(fout_pin, "%s\n", dff[i]->name);
	}

	fclose(fout_pin_input);
	fclose(fout_pin);

	return SUCCESS;
}