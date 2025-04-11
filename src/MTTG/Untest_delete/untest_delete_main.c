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
//�֐���:untest_delete_main
//�@�\:�璷�̏�폜��main
//�߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
//�����F�Ȃ�
//===================================================
int untest_delete_main(void) {
	int i = 0;
	int count;
	int count_a = 0;
	int sat_flag;
	int wsa_flag;
	char cmd[500];
	char txt_cmd[500];
	UNDET_FAULT_LIST* node;
	UNDET_FAULT_LIST* pre_node = NULL;

	node = ufl;

	//�P��̏�e�X�g�p�^�[���t�@�C��
	sprintf(txt_cmd, "%suntest_delete_tp.txt", folder_out);
	if ((fout_untest_tp = (fopen(txt_cmd, "w"))) == NULL) {
		printf("�yERROR�z:File open untest_delete_tp.txt(untest_delete_main.c)\n");
		return ERROR;
	}

	count = 0;

	//�W���o��
	printf("//===========================================\n");
	printf("//Untestable Fault Delete start\n");
	printf("//===========================================\n");

	//progress bar
	printf("|----+----|----+----|----+----|----+----|----+----|\n");

	while (node != NULL) {

		//progress bar
		if (((int)i++ % (int)(((ufl_num + red_num + hpfl_num) * 2 / 100) + 1)) == 0) {
			printf("*");
			count_a++;
		}

		//�̏�͈͓���
		suffix = 2 * n_net + 1;
		specific_fault_range(node->name);

		//PBO�t�@�C��
		sprintf(txt_cmd, "%suntest_pbo.txt",folder_out);
		if ((fout_untest_pbo = fopen(txt_cmd, "w")) == NULL) {
			printf("�yERROR�z:File open untest_pbo.txt(untest_delete_main.c)\n");
			exit(1);
		}

		//PBO�T�C�Y�L�q
		untest_pbo_generate_min(fout_untest_pbo, node);

		//�����H�̋L�q
		pbo_generate_normal_c(fout_untest_pbo);

		//�̏��H�̋L�q
		pbo_generate_fault_c(fout_untest_pbo, node);

		//�̏ጟ�o��H�̋L�q
		pbo_generate_fdetect_c(fout_untest_pbo, node);


		//PBO�t�@�C���N���[�Y
		fclose(fout_untest_pbo);

		//========================================================================
		//	clasp �� �e�X�g�x�N�g������
		//========================================================================
		//Clasp
		sprintf(txt_cmd, "%sclasp_log_untest.txt",folder_out);
		sprintf(cmd, "clasp.exe %suntest_pbo.txt --time-limit %d >%s",folder_out, fi[TIME].value, txt_cmd);
		system(cmd);

		//UNSAT�������ꍇ�A�����o�̏�W������폜
		if ((sat_flag = (unsat_dicision(node, pre_node, 0))) == UNSAT) {
			//UNSAT�̏ꍇ
			if (pre_node == NULL)node = ufl;
			else node = pre_node->next;
			continue;
		}

		if (sat_flag == SAT && ((strcmp(fi[LOW_POWER].name, "OFF")) == 0)) {
			pre_node = node;
			node = node->next;
		}

		if (sat_flag == SAT && ((strcmp(fi[LOW_POWER].name, "ON")) == 0)) {

			//����d�͐����ǉ�

			//PBO�t�@�C��
			sprintf(txt_cmd, "%suntest_pbo.txt",folder_out);
			if ((fout_untest_pbo = fopen(txt_cmd, "a+")) == NULL) {
				printf("�yERROR�z:File open untest_pbo.txt(untest_delete_main.c)\n");
				exit(1);
			}

			//����d�͐���
			pbo_generate_power(fout_untest_pbo);

			//PBO�t�@�C���N���[�Y
			fclose(fout_untest_pbo);

			//========================================================================
			//	clasp �� �e�X�g�x�N�g������
			//========================================================================
			//Clasp
			sprintf(txt_cmd, "%sclasp_log_untest.txt",folder_out);
			sprintf(cmd, "clasp.exe %suntest_pbo.txt --time-limit %d >%s",folder_out, fi[TIME].value, txt_cmd);
			system(cmd);

			//UNSAT�������ꍇ�A�����o�̏�W������폜��������d�͌̏�
			if ((sat_flag = (unsat_dicision(node, pre_node, 2))) == UNSAT) {
				//UNSAT�̏ꍇ
				if (pre_node == NULL)node = ufl;
				else node = pre_node->next;
				continue;
			}
			else if (sat_flag == SAT) {
				//�e�X�g�x�N�g������
				sprintf(cmd, "generate_tv.exe %s %d", txt_cmd, n_pi + n_dff);
				system(cmd);

				//����d�͉��
				if ((wsa_flag = untest_check_wsa(node, pre_node)) == SAT) {
					//�e�X�g�W���ɒǉ�
					make_test_set(fout_untest_tp);
					//SAT�̏ꍇ
					pre_node = node;
					node = node->next;
				}
				else if (wsa_flag == ERROR || wsa_flag == UNSAT) {
					printf("%d", count);
					printf("�yERROR�z:Untest Check WSA(untest_delete_main.c)\n");
					exit(1);
				}
				else if (wsa_flag == UNSAT) {
					printf("�yERROR�z:UNSAT : Untest Check WSA(untest_delete_main.c)\n");
					exit(1);
				}
			}
		}
	}

	printf("\n");
	fclose(fout_untest_tp);

	return SUCCESS;
}