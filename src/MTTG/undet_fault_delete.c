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


////===================================================
////�֐���:undet_fault_delete
////�@�\:�����o�̏჊�X�g����̏���폜
////�߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
////�����F�폜�������̏�
////===================================================
//int undet_fault_delete(UNDET_FAULT_LIST* node) {
//	int i, flag=FLAG_DOWN,flag_1,flag_2;
//	//UNDET_FAULT_LIST *gate_node, *gate_pre_node;
//
//	//1�O�̃m�[�h���i�[����̈��������
//	//gate_pre_node = NULL;
//
//	//�Q�[�g�O���[�v�̕����폜
//	//�Q�[�g���ŒT���J�n
//	/*for (i = 0; i < gate_num; i++) {
//		if (gate_group[i] != NULL) {
//			if (strcmp(gate_group[i]->name, node->name) == 0) {
//				gate_node = gate_group[i];
//				break;
//			}
//		}
//	}*/
//	while (gate_node != NULL && flag == FLAG_DOWN) {
//		flag_1 = 0;
//		flag_2 = 0;
//		//�K�{�����ď�������v���邩�ǂ���
//		for (int k = 0; k < node->input_num; k++) {
//			if (gate_node->input_1[k] == node->input_1[k]) {
//				flag_1++;
//			}
//			if (gate_node->input_2[k] == node->input_2[k]) {
//				flag_2++;
//			}
//		}
//		if ((flag_1 == node->input_num) && (flag_2 == node->input_num)) {
//
//			//�Q�[�g����1�ԍŏ��̃m�[�h
//			if (gate_pre_node == NULL) {
//
//				//�ΏېM�����̍폜
//				gate_pre_node = gate_node->next;
//				free(gate_node);
//				gate_group[i] = gate_pre_node;
//				flag = FLAG_UP;
//				break;
//			}
//
//			//����ȊO
//			else {
//				gate_pre_node->next = gate_node->next;
//				free(gate_node);
//				flag = FLAG_UP;
//				break;
//			}
//		}
//		gate_pre_node = gate_node;
//		gate_node = gate_node->next;
//	}
//
//	return SUCCESS;
//}

//===================================================
// �֐���: undet_fault_delete
// �@�\: �����o�̏჊�X�g����w�肳�ꂽ�J�ڌ̏���폜
// �߂�l�Fint ����I��:SUCCESS(=0), �ُ�I��:ERROR(=-1)
// �����FUNDET_FAULT_LIST* node - �폜�������̏�̃|�C���^
//===================================================
int undet_fault_delete(UNDET_FAULT_LIST* node) {
	UNDET_FAULT_LIST* current, * prev;

	// �����o�̏჊�X�g����A�܂��͍폜�Ώۂ̃m�[�h��NULL�̏ꍇ
	if (ufl == NULL || node == NULL) {
		return ERROR;
	}

	current = ufl;
	prev = NULL;

	// �����o�̏჊�X�g�𑖍�
	while (current != NULL) {
		// �̏�^�C�v�ƃl�b�g������v���邩���m�F
		if (current->type == node->type && strcmp(current->name, node->name) == 0) {
			// �m�[�h���폜
			if (prev == NULL) {
				// �폜����m�[�h���w�b�h�m�[�h�̏ꍇ
				ufl = current->next;
			}
			else {
				prev->next = current->next;
			}
			// �����������
			free(current);
			ufl_num--; // �����o�̏ᐔ���f�N�������g
			return SUCCESS;
		}
		prev = current;
		current = current->next;
	}

	// �m�[�h��������Ȃ������ꍇ
	return ERROR;
}



//========================================================
//�֐���:ufl_free
//�@�\�F�e�X�g�s�\�̏�폜
//�߂�l�Fvoid
//�����F�폜�������̏�C,�e�X�g�s�\(mode=0)��������d�͌̏�(mode=1)�Ȃ̂�
//========================================================
void ufl_free(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node,int mode) {
	UNDET_FAULT_LIST*p_node;

	p_node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

	//���i�[
	strcpy(p_node->name, node->name);
	p_node->type=node->type;
	p_node->netlist = node->netlist;

	if (mode == 0) {//�e�X�g�s�\
		//p_node��red�܂���hpfl�̐擪�ɒǉ����Ă���
		p_node->next = red;
		red = p_node;
	}
	else if (mode == 1) {//������d��
		p_node->next = hpfl;
		hpfl = p_node;
	}

	//�폜(free)
	if (pre_node == NULL) {

		ufl = node->next;

		ufl_num--;

		if (mode == 0) {
			red_num++;
		}
		else if (mode == 1) {
			hpfl_num++;
		}

		free(node);

	}
	else {
		pre_node->next = node->next;

		ufl_num--;

		if (mode == 0) {
			red_num++;
		}
		else if (mode == 1) {
			hpfl_num++;
		}

		free(node);

	}

	return;
}