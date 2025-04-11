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
//�֐���:levelize
//�@�\:���x���C�Y
//�߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
//�����F�Ȃ�
//===================================================
int levelize(void) {

	int i;
	int max_in_lev = 0;	//�ő���̓Q�[�g���x��
	NLIST* nlist;		//NLIST


	//(�^��)�O�����͂��X�^�b�N�ɓ����
	//�O������
	for (i = 0; i < n_pi; i++) { 
		level_push(pi[i]);
	}
	//�O������
	for (i = 0; i < n_dff; i++) {
		level_push(dff[i]);
	}

	while (Level.ptr != 0) {

		nlist = level_pop();

		switch (nlist->type) {

		case IN:
			nlist->level = 1;		//���x���ݒ�
			nlist->flag = 1;		//�t���O�ݒ�
			for (i = 0; i < nlist->n_out; i++) {
				level_push(nlist->out[i]);//�o�̓v�b�V��
			}
			break;

		case DFF:
			nlist->level = 1;		//���x���ݒ�
			nlist->flag = 1;		//�t���O�ݒ�
			for (i = 0; i < nlist->n_out; i++) {
				level_push(nlist->out[i]);//�o�̓v�b�V��
			}
			break;

		default:

			//���͐��̃��x������
			if (judge_in_level(nlist->in, nlist->n_in) == SUCCESS) {

				//�ő�̓��̓��x���Z�o
				for (i = 0; i < nlist->n_in; i++) {
					if (max_in_lev < nlist->in[i]->level) {
						max_in_lev = nlist->in[i]->level;
					}
				}

				nlist->level = max_in_lev + 1;	//���x���ݒ�
				nlist->flag = 1;				//�t���O�ݒ�
				max_in_lev = 0;					//�ϐ�������

				if (nlist->ppo_flag != 1) {
					for (i = 0; i < nlist->n_out; i++) {
						level_push(nlist->out[i]);	//�o�̓v�b�V��
					}
				}

			}
			break;

		}

	}


	//---------------------------------------------
	//Q�\�[�g
	//---------------------------------------------
	Qsort();

	//---------------------------------------------
	//�Y�����iID�j�̐ݒ�
	//---------------------------------------------
	set_literal_id();

	//---------------------------------------------
	//�t���O���Z�b�g
	//---------------------------------------------
	reset_flag();

	return SUCCESS;

}

//=========================================================================
//  �֐��� : judge_in_level
//  �@  �\ : ���͐��̃��x���C�Y�����̔���
//  �߂�l : 1(��) / 0(��)
//  ��  �� : ���̓|�C���^�z��C���͐�
//=========================================================================
int judge_in_level(NLIST **in_pin, int n) {

	int i = 0;

	//�S���͐��̃��x���t���͊������Ă��邩����
	while (i < n) {
		if (in_pin[i]->flag != 1) {
			return FAILURE;	//���s
		}
		i++;
	}

	return SUCCESS;	//����

}