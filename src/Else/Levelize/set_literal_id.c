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
//  �֐��� : set_literaid
//  �@  �\ : ���e����ID�ݒ�
//  �߂�l : �Ȃ�
//  ��  �� : �Ȃ�
//=========================================================================
void set_literal_id(void) {

	int i;

	//�Y�����ݒ�
	for (i = 0; i < n_pi; i++) {
		pi[i]->suf_1t = i + 1;
		pi[i]->suf_2t = i + 1 + n_net;
		//printf("1�����ځF%d  ", pi[i]->suf_1t);
		//printf("2�����ځF%d  ", pi[i]->suf_2t);
		//printf("���O�F%s\n", pi[i]->name);
	}
	for (i = 0; i < n_dff; i++) {
		dff[i]->suf_1t = i + n_pi + 1;
		dff[i]->suf_2t = i + n_pi + 1 + n_net;
		//printf("1�����ځF%d  ", dff[i]->suf_1t);
		//printf("2�����ځF%d  ", dff[i]->suf_2t);
		//printf("���O�F%s\n", dff[i]->name);
	}
	for (i = n_pi + n_dff; i < n_net; i++) {
		sort_nl[i]->suf_1t = i + 1;
		sort_nl[i]->suf_2t = i + 1 + n_net;
		//printf("1�����ځF%d  ", sort_nl[i]->suf_1t);
		//printf("2�����ځF%d  ", sort_nl[i]->suf_2t);
		//printf("���O�F%s\n", sort_nl[i]->name);
	}

}