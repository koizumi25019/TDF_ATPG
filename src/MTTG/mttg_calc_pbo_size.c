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

//=========================================================================
//  �֐��� : mttg_calc_pbo_size
//  �@  �\ : PBO�̃T�C�Y���v�Z
//  �߂�l : �Ȃ�
//  ��  �� : �Ȃ�
//=========================================================================
PBO mttg_calc_pbo_size(void) {

	PBO pbo;
	pbo.val = 0;
	pbo.h_constraint = 0;
	pbo.s_constraint = 0;
	suffix = n_net*2+1;



	//---------------------------------------------
	//���񐔌v�Z
	//---------------------------------------------
	//�n�[�h����(�����H�E�̏��H�E�̏ጟ�o��H�j���v�Z
	pbo = mttg_calc_hard_constraint();
	//�\�t�g����(tfl�̎c��̌̏ጟ�o��N�j���v�Z
	pbo.s_constraint = mttg_calc_soft_constraint();

	if (strcmp(fi[LOW_POWER].name, "OFF") == 0) {
		pbo.h_constraint += pbo.s_constraint;
		//---------------------------------------------
		//�ϐ��v�Z(tfl_num!=1)
		//---------------------------------------------
		if (tfl_num != 1) {
			pbo.val += pbo.s_constraint;
		}
	}
	//�\�t�g����(����d�͐���)���v�Z(����d�͍l������)
	if (strcmp(fi[LOW_POWER].name , "ON")==0) {
		pbo.s_constraint += untest_calc_soft_constraint();
		pbo.h_constraint += pbo.s_constraint;
		//---------------------------------------------
		//�ϐ��v�Z(tfl_num!=1)
		//---------------------------------------------
		if (tfl_num != 1) {
			pbo.val += pbo.s_constraint;
		}
	}

	//---------------------------------------------
	//�ϐ��v�Z(tfl_num==1)
	//---------------------------------------------
	if (tfl_num == 1) {
		if (fc_ppo == 1) {
			pbo.val = suffix + fc_ppo + pbo.s_constraint - 1;
		}
		else if (fc_ppo != 1) {
			pbo.val = suffix + fc_ppo + 1 + pbo.s_constraint;
		}
	}

	return pbo;

}


//=========================================================================
//  �֐��� : mttg_calc_hard_constraint
//  �@  �\ : �ϐ��E���񐔌v�Z(�n�[�h)
//  �߂�l : PBO�T�C�Y
//  ��  �� : �Ȃ�
//=========================================================================
PBO mttg_calc_hard_constraint(void) {
	int i,j=0;
	int fault_det;
	PBO p;
	UNDET_FAULT_LIST* head;
	UNDET_FAULT_LIST* tmp;

	p.val = 0;
	p.h_constraint = 0;

	head = tfl;

	//-------------------------------------------------------------------------------------
	//�����H�ɂ���
	//-------------------------------------------------------------------------------------
	//-------------------------------------------------------------------
	//	�����H�̐��񐔂��v�Z
	//  �����H�̐���͘_���Q�[�g�̊�{�����\������̂ŁA���f���ɂ�炸����
	//-------------------------------------------------------------------
	for (i = 0; i < n_net; i++) {
		switch (nl[i].type) {
		case AND:
			p.h_constraint += nl[i].n_in + 1;
			break;
		case NAND:
			p.h_constraint += nl[i].n_in + 1;
			break;
		case OR:
			p.h_constraint += nl[i].n_in + 1;
			break;
		case NOR:
			p.h_constraint += nl[i].n_in + 1;
			break;
		case INV:
			p.h_constraint += 2;
			break;
		case FOUT:
			p.h_constraint += 2;
			break;
		case BUF:
			p.h_constraint += 2;
			break;
		case EXOR:
			p.h_constraint += 4;
			break;
		}
	}
	//2�����ڂ̐����H
	p.h_constraint = p.h_constraint * 2;
	//1�����ڂ�2�����ڂ�FF��ڑ�����DFF
	p.h_constraint += 2 * n_dff;
	//1�����ڂ�2�����ڂ�PI��ڑ�����DFF
	p.h_constraint += n_pi * 2;

	//-------------------------------------------------------------------------------------
	//�̏��H�ɂ���
	//-------------------------------------------------------------------------------------
	while (head != NULL) {
		tmp = head->next;	//tmp�Ɏ��̖ڕW�̏�m�[�h��ۑ�
		specific_fault_range(head->name);	//�̏�͈͂̓���
		//-------------------------------------------------------------------
		//	�̏��H�̐��񐔂��v�Z
		//-------------------------------------------------------------------
		for (i = 0; i < n_net; i++) {
			if (nl[i].fault_pass == YES && strcmp(nl[i].name, head->name) != 0) {
				switch (nl[i].type) {
				case AND:
					p.h_constraint += nl[i].n_in + 1;
					break;
				case NAND:
					p.h_constraint += nl[i].n_in + 1;
					break;
				case OR:
					p.h_constraint += nl[i].n_in + 1;
					break;
				case NOR:
					p.h_constraint += nl[i].n_in + 1;
					break;
				case INV:
					p.h_constraint += 2;
					break;
				case FOUT:
					p.h_constraint += 2;
					break;
				case BUF:
					p.h_constraint += 2;
					break;
				case EXOR:
					p.h_constraint += 4;
					break;
				}
			}
		}
		//-------------------------------------------------------------------------------------
		// �̏ጟ�o��H�ɂ���
		//-------------------------------------------------------------------------------------
		//-------------------------------------------------------------------
		//	�̏ጟ�o��H�̐��񐔂��v�Z
		//-------------------------------------------------------------------
		//PPO���P��̏ꍇ�yEXOR�P���z
		if (fc_ppo == 1) { p.h_constraint += 4; }
		//PPO�������̏ꍇ�yEXOR����(PPO����)��OR�Q�[�g�z
		else if (fc_ppo != 1) { p.h_constraint += fc_ppo * 4 + fc_ppo + 1; }
		//-------------------------------------------------------------------
		//	�ϐ��̐����v�Z
		//-------------------------------------------------------------------
		//tfl����������ꍇ
		if (tfl_num != 1) {
			if (fc_ppo == 1)p.val += suffix - p.val; 	//PPO���P��̏ꍇ
			else if (fc_ppo != 1)p.val += suffix + fc_ppo - p.val;	//PPO�������̏ꍇ*/
		}
		//---------------------------------------------
		//�̏��N�ɂ���
		//---------------------------------------------
		//1�ڂ����n�[�h�c��\�t�g
		if (j == 0) {
			fault_det = /*head->input_num * 2 + */3 + 1;
			p.h_constraint += fault_det;
		}
		j++;

		//�Y�����ݒ�(tfl_num!=1)
		if (tfl_num != 1) {
			suffix = p.val + 1;
		}

		//���̌̏��
		head = tmp;
	}

	//�߂�l�F�ϐ��̐��E����
	return p;
}

//=========================================================================
//  �֐��� : mttg_calc_soft_constraint
//  �@  �\ : ���񐔌v�Z(�\�t�g�@1�ڈȊO�̖ڕW�̏��N)
//  �߂�l : �Ȃ�
//  ��  �� : �Ȃ�
//=========================================================================
int mttg_calc_soft_constraint(void) {
	UNDET_FAULT_LIST* head;
	int n = 0;
	int fault_det;

	head = tfl->next;

	while (head != NULL) {
		fault_det = 3 + 1;
		n += fault_det;
		head = head->next;
	}

	return n;
}