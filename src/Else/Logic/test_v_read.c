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

//=========================================================================
//  �֐��� : test_v_read
//  �@  �\ : �e�X�g�x�N�g���}��
//�@�߂�l�Fint ����I��:SUCCESS �ُ�I��:ERROR
//�@�����Fvoid
//=========================================================================
int test_v_read(void) {
	int i,j;
	FILE *fp;
	char *str;
	char *p_str;

	//fopen
	fp = fopen("tv.txt", "r");

	//�e�X�g�p�^�[���̈�m��
	str = (char*)malloc(sizeof(char)*((n_pi + n_ppi) + 1));

	//�e�X�g�p�^�[���Ǎ�
	fgets(str, (n_pi + n_ppi + 1), fp);

	//���s�R�[�h�؂�o��
	p_str = strtok(str, "\n");

	//�i�[
	for (i = 0; i < n_pi ; i++) {
		pi[i]->val_1t = p_str[i] - 48;
	}
	for (j = 0; j < n_ppi; j++) {
		ppi[j]->val_1t = p_str[i] - 48;
		i++;
	}

	//fclose
	fclose(fp);

	//free
	free(str);

	return SUCCESS;
}