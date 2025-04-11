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
#include"../../Header/Define.h"

//===================================================
//�֐���:fault_judge
//�@�\:���o�̏����
//�߂�l�F���o���ꂽ�̏ᐔ
//�����F�Ȃ�
//===================================================
int fault_judge(void) {
	UNDET_FAULT_LIST *d_head;
	UNDET_FAULT_LIST *u_head;
	UNDET_FAULT_LIST *df_node;
	UNDET_FAULT_LIST *pre_node;
	UNDET_FAULT_LIST *node;
	char str[256];
	char txt_cmd[300];
	char* p_str_type;
	int  p_type;
	char* p_str_net;
	int i = 1;
	int j, k;
	int val_1[1000];
	int val_2[1000];
	int count = 0;
	int det_fault_num_clasp = 0;//1�e�X�g�p�^�[���ł̌��o�̏ᐔ

	//�̏Ꭻ���t�@�C���I�[�v��
	sprintf(txt_cmd, "%sfsim_log.txt", folder_out);
	if ((fout_fsim_result = fopen(txt_cmd, "r")) == NULL) {
		printf("�yERROR�z:File open fsim_log.txt(fault_judge.c)\n");
		return ERROR;
	}

	d_head = dfl;

	//----------------------------------------
	//tp0
	//STR G9
	//STF G6
	//STF G8
	//STF G16
	//STF G8_G16_B
	//----------------------------------------

	while (fgets(str, 256, fout_fsim_result) != NULL) {
		if (ufl_num == 0)break;
		if (i != 1 && strcmp(str,"\n")!=0) { 
			p_str_type= strtok(str, " \t\n"); //STR or STF
			if (strcmp(p_str_type, "STR") == 0) {
				p_type = STR;
			}
			else if (strcmp(p_str_type, "STF") == 0) {
				p_type = STF;
			}
			p_str_net = strtok(NULL, " \t\n"); //G9
			for (j = 0; j < n_net; j++) {
				if (strcmp(p_str_net, nl[j].name) == 0) {
					for (k = 0; k < nl[j].n_in; k++) {
						val_1[k] = nl[j].in[k]->val_1t;
						val_2[k] = nl[j].in[k]->val_2t;
					}
					break;
				}
			}

			u_head = ufl;

			//�m�[�h��1�ԍŏ������o�����ꍇ�̏���
			if (p_type == u_head->type) {
				if (strcmp(u_head->name, p_str_net) == 0) {

					//���o�̏�m�[�h
					df_node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

					//���R�s�[
					strcpy(df_node->name, u_head->name);
					df_node->type=u_head->type;
					df_node->netlist = u_head->netlist;

					//�ڕW�̏�ݒ�
					if (u_head->netlist->tf_flag == FLAG_UP) {
						count++;//�ڕW�̏Ⴊ���o���ꂽ�ꍇcount����
					}

					//���`���X�g����
					df_node->next =d_head;
					d_head = df_node;

					//���o�A�����o�̏ᐔ�X�V
					dfl_num++;
					det_fault_num_clasp++;

					//���o�̏�𖢌��o�̏჊�X�g����폜
					ufl_free(u_head, NULL, -1);
					/*pre_node = u_head->next;
					free(u_head);
					ufl = pre_node;*/

					continue;
				}
			}

			pre_node = ufl;
			u_head = ufl->next;
			//����ȊO�̏���
			while (u_head != NULL) {
				if (p_type == u_head->type) {
					if (strcmp(u_head->name, p_str_net) == 0) {

						//���o�̏�m�[�h�v
						df_node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));

						//���R�s�[
						strcpy(df_node->name, u_head->name);
						df_node->type = u_head->type;
						df_node->netlist = u_head->netlist;

						//�ڕW�̏�ݒ�
						if (u_head->netlist->tf_flag == FLAG_UP) {
							count++;
						}

						//���`���X�g����
						df_node->next = d_head;
						d_head = df_node;

						//���o�A�����o�̏ᐔ�X�V
						dfl_num++;
						det_fault_num_clasp++;

						//���o�̏�𖢌��o�̏჊�X�g����폜
						ufl_free(u_head, pre_node, -1);
						/*pre_node = u_head->next->next;
						free(u_head->next);
						u_head->next = pre_node;*/

						break;
					}
				}
				pre_node = pre_node->next;
				u_head = u_head->next;
			}
		}
		i++;
	}

	//dfl ���o�̏჊�X�g�̍X�V
	dfl = d_head;

	//�ڕW�̏�폜
	node = tfl;
	while (node != NULL) {
		d_head = node->next;
		free(node);
		node = d_head;
	}

	// �̏ጟ�o������e�X�g�p�^�[�������C���N�������g
	if (det_fault_num_clasp > 0) {
		tp_num++;
	}

	//�W���o��
	standard_output();

	//�t�@�C������
	fclose(fout_fsim_result);

	sprintf(txt_cmd, "%smttg.txt", folder_out);
	fout_mttg = fopen(txt_cmd, "a+");
	fprintf(fout_mttg, "%d: tfl_num:%d\ttfl_det_fault:%d\tdet_fault:%d\n", tp_num, tfl_num, count, det_fault_num_clasp);
	fclose(fout_mttg);
	
	return det_fault_num_clasp;
}