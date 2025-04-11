//------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//------------------------------------------------------------------------
//����C���N���[�h
//------------------------------------------------------------------------
#include"../Header/Define.h"

//===================================================
//�֐���:Transition_Delay_Fault_Set
//�@�\:�J�ڌ̏�S�̏�W���쐬
//�߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
//�����F�Ȃ�
//===================================================
int all_transition_delay_fault_set(void) {
	int i;
	int id = 0;
	UNDET_FAULT_LIST* head, * node;

	//���`���X�g�̏�����
	head = NULL;

	for (i = 0; i < n_net; i++) {
		switch (sort_nl[i]->type) {
		case AND:
		//STR
		//�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//���i�[
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STR;

		//�S�̏�ID����
	    node->node_id = id;
        id++;

		//���`���X�g�\�z
	    node->next = head;
		//�ŏ���node�̎�O�ɐV����node���ǉ�����Ă����C���[�W
		//head�F���݂̃��X�g�̐擪���w��
		//���̑���ɂ��,�V�����쐬���ꂽnode�����݂̃��X�g�̐擪�m�[�h�Ƃ��Ĉ�����
		head = node;
		//�ŏ�NULL������head�Ɍ��݂�node(���A�l�����Ă�����node)�̃A�h���X��n���Ă���
		ufl_num++;

		//STF
        //�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STF;

		//�S�̏�ID����
		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		//��قǂ����Ă���head�����݂�node->next�Ƃ��邱�ƂŁA���݂�node�̌��ɑO��node�������N���邱�Ƃ��ł���
		head = node;
		//head�Ɍ��݂�node�̃A�h���X��^����
		ufl_num++;
		break;

		case NAND:
		//STR
        //�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//���i�[
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STR;

		//�S�̏�ID����
		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		//STF
        //�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//���i�[
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STF;

		//�S�̏�ID����
		node->node_id = id;
	    id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		break;

		case OR:
		//STR
        //�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//���i�[
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STR;

		//�S�̏�ID����
		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		//STF
        //�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//���i�[
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STF;

		//�S�̏�ID����
		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		break;

		case NOR:
		//STR
		//�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//���i�[
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STR;

		//�S�̏�ID����
		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		//STF
	    //�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//���i�[
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STF;

		//�S�̏�ID����
		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		break;

		case INV:
		//STR
	    //�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//���i�[
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STR;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		//STF
		//�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//���i�[
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STF;

		//�S�̏�ID����
		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;
		break;

		case BUF:
		//STR
		//�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//���i�[
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STR;

		//�S�̏�ID����
		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		//STF
		//�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		//���i�[
		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STF;

		//�S�̏�ID����
		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		break;

		case FOUT:
		//STR
		//�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STR;

		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STF;

		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		break;

		case IN:
        //STR
        //�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STR;

		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

	    node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STF;

		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		break;

		case DFF:
		//STR
		//�m�[�h�쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STR;

		node->node_id = id;
		id++;

	    //���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

		strcpy(node->name, sort_nl[i]->name);
		node->netlist = sort_nl[i];

		//�̏�^�C�v�̊i�[
		node->type = STF;

		node->node_id = id;
		id++;

		//���`���X�g�\�z
		node->next = head;
		head = node;
		ufl_num++;

		break;

		default:
		printf("�z�肵�Ă��Ȃ��Q�[�g�^�C�v�ł�(default)");
		break;
		}
	}

	//�S�M�����̏����I��
	ufl = head;
	afl = head;
	node_num = id;

	return SUCCESS;
}


//===================================================
// �֐���:Input_Transition_Delay_Fault_Set
// �@�\:�J�ڌ̏�̖����o�̏�W�����\���̂Ɋi�[
// �߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
// �����F�Ȃ�
//===================================================

int input_transition_delay_fault_set(void) {

	int id = 0;
	char line[100];//�t�@�C����1�s�����擾
	char fault_type[10];
	char net_name[50];
	//char file_name[50];
	UNDET_FAULT_LIST* head = NULL;
	UNDET_FAULT_LIST* node;
	NLIST* net_info; 

	//�����o�̏�W���t�@�C���I�[�v��
	FILE* fp = fopen(fi[FAULT].name, "r");
	if (fp == NULL) {
		printf("�yERROR�z: File open failed (%s)\n", fi[FAULT].name);
		return ERROR;
	}

	// �t�@�C������e�s��ǂݎ���ĉ��
	while (fgets(line, sizeof(line), fp) != NULL) {//�t�@�C����1�s�����擾
		if (sscanf(line, "%s %s", net_name,fault_type) != 2) {
			printf("�yWARNING�z: Invalid format in line: %s", line);
			continue;//���݂̔���(itertion)���X�L�b�v���A���̔����Ɉڂ�
		}

		// �l�b�g������Ƀ\�[�g�ς݃l�b�g���X�g����Y���������T��
		net_info = search_netlist(net_name); 
		if (net_info == NULL) {
			printf("�yERROR�z: Net not found (%s)\n", net_name);
			continue;
		}

		// �V���������o�̏�m�[�h���쐬
		node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));
		if (node == NULL) {
			printf("�yERROR�z: Memory allocation failed\n");
			fclose(fp);
			return ERROR;
		}

		// �̏���̊i�[ (NLIST �̏��� UNDET_FAULT_LIST �ɔ��f)
		node->netlist = net_info;  // NLIST �̃|�C���^�����̂܂܎g�p�\
		strcpy(node->name, net_name);

		// �̏�^�C�v�̐ݒ�
		if (_stricmp(fault_type, "STR") == 0) {
			node->type = STR;
		}
		else if (_stricmp(fault_type, "STF") == 0) {
			node->type = STF;
		}
		else {
			printf("�yWARNING�z: Unknown fault type (%s)\n", fault_type);
			free(node);
			continue;
		}

		//�M�����̏�ID����
		node->node_id = id;
		id++;

		// ���X�g�̐擪�ɒǉ�
		node->next = head;
		head = node;
		ufl_num++;
	}

	fclose(fp);

	// ufl �Ƀ��X�g���i�[
	ufl = head;
	afl = head; 
	node_num = id;

	printf("�yINFO�z: Transition fault set created successfully.\n");
	return SUCCESS;
}

//===================================================
// �֐���:search_netlist
// �@�\:�l�b�g������\�[�g�l�b�g���X�g��T�����A�Y������m�[�h�̏���Ԃ�
// �߂�l�FUNDET_FAULT_LIST* �Y������m�[�h, ������Ȃ��ꍇ��NULL
// �����Fchar* name - �l�b�g��
//===================================================
NLIST* search_netlist(char* name) {
	for (int i = 0; i < n_net; i++) {
		if (strcmp(sort_nl[i]->name, name) == 0) {
			return sort_nl[i];
		}
	}
	return NULL;
}
