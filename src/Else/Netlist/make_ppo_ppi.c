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
//�֐���:make_ppo_ppi
//�@�\:�^���O�����o�͔z��̍쐬
//�߂�l�F�Ȃ�
//�����F�Ȃ�
//===================================================
void make_ppo_ppi(void) {

	int i;
	int count_ppo = 0;
	int count_ppi = 0;

	//�̈�m��
	ppo = (NLIST**)malloc(sizeof(NLIST*)*n_dff);
	if (ppo == NULL) {
		printf("failed to allocate memory for ppo\n");
		exit(EXIT_FAILURE);
	}
	ppi = (NLIST**)malloc(sizeof(NLIST*)*n_dff);
	if (ppi == NULL) {
		printf("failed to allocate memory for ppi\n");
		exit(EXIT_FAILURE);
	}


	//ppi����
	for (i = 0; i < n_net; i++) {
		if (nl[i].type == DFF) {
			ppi[count_ppi++] = &nl[i];
		}
	}

	//ppo����
	for (i = 0; i < count_ppi; i++) {
		ppo[count_ppo++] = ppi[i]->in[0];
		ppo[i]->ppo_flag = 1;
	}

	//�����i�[
	n_ppi = count_ppi;
	n_ppo = count_ppo;

	//�^���O���o�͐��ݒ�
	n_ppo = n_dff;

	return SUCCESS;

}