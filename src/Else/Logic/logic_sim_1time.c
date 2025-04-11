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
//  �֐��� : logic_sim_1time
//  �@  �\ : 1�����ژ_���V�~�����[�V����
//�@�߂�l�Fint ����I��:SUCCESS �ُ�I��:ERROR
//�@�����Fvoid
//=========================================================================
int logic_sim_1time(void) {
	int i;

	for (i = n_pi + n_ppi; i < n_net; i++) {
		switch (sort_nl[i]->type) {
		case AND:
			sort_nl[i]->val_1t = and (sort_nl[i], 1);
			break;

		case NAND:
			sort_nl[i]->val_1t = nand(sort_nl[i], 1);
			break;

		case OR:
			sort_nl[i]->val_1t = or(sort_nl[i], 1);
			break;

		case NOR:
			sort_nl[i]->val_1t = nor(sort_nl[i], 1);
			break;

		case INV:
			sort_nl[i]->val_1t = inv(sort_nl[i], 1);
			break;

		case FOUT:
			sort_nl[i]->val_1t = fout(sort_nl[i], 1);
			break;

		case DFF:
		case DFFS:
			break;


		default:
			puts("���Ή�");
			return ERROR;
		}
	}

	return SUCCESS;
}