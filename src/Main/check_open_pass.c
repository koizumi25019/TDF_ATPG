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
//�֐���:check_open_pass
//�@�\:�I�[�v���p�X�̃`�F�b�N
//�߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
//�����F�Ȃ�
//===================================================
int check_open_pass(void) {
	int i;
	NLIST *p;

	//FF��蓞�B�\�ȐM�����̒T��
	for (i = 0; i < n_dff; i++) { open_push(dff[i]); }
	while (Open.ptr != 0) {
		p = open_pop();
		if (p->open_pass == YES) {
			p->open_pass = NO;	//�I�[�v���p�X�łȂ�
			p->fault_pass = YES;//�I�[�v���p�X�łȂ�
			if (p->n_out != 0 && p->ppo_flag != 1) {
				for (i = 0; i < p->n_out; i++) { open_push(p->out[i]); }
			}
		}
	}
	return SUCCESS;
}