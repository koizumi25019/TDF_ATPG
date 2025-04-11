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
//  �֐��� : untest_cloc_pbo_size
//  �@  �\ : PBO�̃T�C�Y���v�Z
//  �߂�l : �Ȃ�
//  ��  �� : �Ώی̏�
//=========================================================================
PBO untest_calc_pbo_size(UNDET_FAULT_LIST* node) {

	PBO pbo;
	pbo.val = 0;
	pbo.h_constraint = 0;
	pbo.s_constraint = 0;
	pbo.lit = 0;


	//---------------------------------------------
	//���񐔌v�Z
	//---------------------------------------------
	pbo.h_constraint = untest_calc_hard_constraint(node);  //�n�[�h����(�e�X�g�������f��)
	pbo.s_constraint = untest_calc_soft_constraint();	    //�\�t�g����(����d�͐���)
	pbo.h_constraint += pbo.s_constraint;

	//---------------------------------------------
	//�ϐ��v�Z
	//---------------------------------------------
	if (fc_ppo == 1) {
		pbo.val = suffix + fc_ppo + pbo.s_constraint - 1;
	}
	else if (fc_ppo != 1) {
		pbo.val = suffix + fc_ppo + 1 + pbo.s_constraint;
	}

	return pbo;

}


//=========================================================================
//  �֐��� : untest_calc_hard_constraint
//  �@  �\ : ���񐔌v�Z(�n�[�h)
//  �߂�l : �Ȃ�
//  ��  �� : �Ώی̏�
//=========================================================================
int untest_calc_hard_constraint(UNDET_FAULT_LIST* node) {

	unsigned int n = 0;
	int fault_det;

	//---------------------------------------------
	//�����H
	//---------------------------------------------
	for (int i = 0; i < n_net; i++) {
		switch (nl[i].type) {
		case AND:
			n += nl[i].n_in + 1;
			break;
		case NAND:
			n += nl[i].n_in + 1;
			break;
		case OR:
			n += nl[i].n_in + 1;
			break;
		case NOR:
			n += nl[i].n_in + 1;
			break;
		case INV:
			n += 2;
			break;
		case FOUT:
			n += 2;
			break;
		case BUF:
			n += 2;
			break;
		case EXOR:
			n += 4;
			break;
		}
	}
	n = n * 2;	//2������

	//---------------------------------------------
	//�̏��H
	//---------------------------------------------

	for (int i = 0; i < n_net; i++) {
		if (nl[i].fault_pass == YES && nl[i].suf_fc != n_net * 2 + 1) {
			switch (nl[i].type) {
			case AND:
				n += nl[i].n_in + 1;
				break;
			case NAND:
				n += nl[i].n_in + 1;
				break;
			case OR:
				n += nl[i].n_in + 1;
				break;
			case NOR:
				n += nl[i].n_in + 1;
				break;
			case INV:
				n += 2;
				break;
			case FOUT:
				n += 2;
				break;
			case BUF:
				n += 2;
				break;
			case EXOR:
				n += 4;
				break;
			}
		}
	}

	//---------------------------------------------
	//�̏ጟ�o��H
	//---------------------------------------------
	//PPO��1�̏ꍇ�yEXOR�P�̂ݒǉ��z
	if (fc_ppo == 1) { n += 4; }
	//PPO��2�ȏ�̏ꍇ�yEXOR�iPPO�����j�� OR�Q�[�g�z
	else if (fc_ppo != 1) { n += fc_ppo * 4 + fc_ppo + 1; }

	//---------------------------------------------
	//1�����ڂ�2�����ڂ�FF��ڑ�����DFF
	//---------------------------------------------
	n = n + 2 * n_dff;

	//---------------------------------------------
	//1�����ڂ�2�����ڂ�PI��ڑ�����DFF
	//---------------------------------------------
	n = n + n_pi * 2;

	//---------------------------------------------
	//�̏��N��
	//---------------------------------------------
	fault_det = 3 + 1; //STF or STR �{det=1
	n += fault_det;

	//�߂�l�F����
	return n;
}

//=========================================================================
//  �֐��� : untest_calc_soft_constraint
//  �@  �\ : ���񐔌v�Z(�\�t�g�@����d��)
//  �߂�l : �Ȃ�
//  ��  �� : �Ȃ�
//=========================================================================
int untest_calc_soft_constraint(void) {

	int n = 0;

	for (int i = 0; i < n_net; i++) {
		if (nl[i].open_pass == NO) n++;
	}

	return n;
}