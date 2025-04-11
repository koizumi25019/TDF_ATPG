//------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include<stdlib.h>
#include<string.h>
#include<direct.h>

//------------------------------------------------------------------------
//����C���N���[�h
//------------------------------------------------------------------------
#include"../../Header/Define.h"

//===================================================
//�֐���:init_exturn
//�@�\:�������֘A
//�߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
//�����F�Ȃ�
//===================================================
int init_exturn(void) {
	char txt_cmd[300];
	//char* file_n;
	int str_file;

	//�J�ڌ̏�֘A
	red = NULL;
	hpfl = NULL;
	ufl_num = 0;		       //�����o�̏ᐔ
	dfl_num;				   //���o�̏ᐔ
	red_num = 0;		       //�e�X�g�s�\�̏ᐔ
	hpfl_num = 0;			   //������d�͌̏ᐔ

	//���̑�
	tp_num = 0;					//�e�X�g�x�N�g����

	//�L���[
	Que.max = n_net * 1000;	    //�ő�l
	Que.front = 0;				//�擪�C���f�b�N�X
	Que.rear = 0;				//�����C���f�b�N�X
	Que.num = 0;				//�v�f��
	if ((Que.que = (NLIST**)calloc(n_net * 1000, sizeof(NLIST*))) == NULL) {
		printf("�yERROR�z:No calloc Que(init_exturn.c)");
		exit(EXIT_FAILURE);
	}

	//���x���C�Y
	Level.max_num = n_net * 100;	//�ő�l
	Level.ptr = 0;					//�C���f�b�N�X
	if ((Level.stk = (NLIST**)calloc(n_net, sizeof(NLIST*))) == NULL) {
		printf("�yERROR�z:No calloc Level stack(init_exturn.c)\n");
		return ERROR;
	}

	//�I�[�v���p�X
	Open.max_num = n_net * 100;		//�ő�l
	Open.ptr = 0;					//�C���f�b�N�X
	if ((Open.stk = (NLIST**)calloc(n_net, sizeof(NLIST*))) == NULL) {
		printf("�yERROR�z:No calloc Open stack(init_exturn.c)\n");
		return ERROR;
	}

	//�t�H���_�[�̍쐬
	if ((folder_out = (char*)calloc(100, sizeof(char))) == NULL) {
		printf("�yERROR�z:No calloc Folder name output(init_exturn.c)\n");
		return ERROR;
	}
	strcpy(folder_out,fi[FOLDER].name);
	str_file = strlen(fi[FOLDER].name) - 1;
	fi[FOLDER].name[str_file] = '\0';

	if (_mkdir(fi[FOLDER].name) != 0) {
		printf("�yERROR�z:Please Delete Same name Folder(init_exturn.c)\n");
		return ERROR;
	}

	//�t�@�C����������
	sprintf(txt_cmd, "%smttg.txt", folder_out);;
	if ((fout_mttg = fopen(txt_cmd, "w")) == NULL) {
		printf("�yERROR�z:File open mttg.txt(init_exturn.c)\n");
		return ERROR;
	}
	fclose(fout_mttg);

	sprintf(txt_cmd, "%stfl_log.txt", folder_out);
	if ((fout_tfl_log = fopen(txt_cmd, "w")) == NULL) {
		printf("�yERROR�z:File open tfl_log.txt(init_exturn.c)\n");
		return ERROR;
	}
	fclose(fout_tfl_log);

	return SUCCESS;
}