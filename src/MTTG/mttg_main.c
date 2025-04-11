//------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

//------------------------------------------------------------------------
//����C���N���[�h
//------------------------------------------------------------------------
#include"../Header/Define.h"

//===================================================
//�֐���:mttg_main
//�@�\:�J�ڌ̏��MTTG�̃��C��
//�߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
//�����F�Ȃ�
//===================================================
int mttg_main(void) {
	int i = 0;
	char txt_cmd[200];
	char cmd[500];
	int unsat_flag = OFF;
	UNDET_FAULT_LIST *head;
	int det_fault_clasp_num;

	//�e�X�g�p�^�[���t�@�C���I�[�v��
	sprintf(txt_cmd, "%stp_set.txt", folder_out);
	if ((fout_tp = (fopen(txt_cmd, "w"))) == NULL) {
		printf("�yERROR�z:File open tp_set.txt(mttg_main.c)\n");
		return ERROR;
	}
	//PIN�t�@�C������
	if (pin_make() != SUCCESS) {
		printf("�yERROR�z:pin make(mttg_main.c)\n");
		return ERROR;
	}

	//���x���C�Y
	if (levelize() != SUCCESS) {
		printf("�yERROR�z:levelize(mttg_main.c)\n");
		return ERROR;
	}

	//�����o�̏჊�X�g�̍쐬
	if(fi[FAULT].flag==ON){
		if (input_transition_delay_fault_set() != SUCCESS) {
			printf("�yERROR�z:Transition_Delay_Fault_Set(mttg_main.c)\n");
			return ERROR;
		}
	}
	else {
		if (all_transition_delay_fault_set() != SUCCESS) {
			printf("�yERROR�z:Transition_Delay_Fault_Set(mttg_main.c)\n");
			return ERROR;
		}
	}
	//�S�̏ᐔ�̊i�[
	all_num = ufl_num;

	//�e�X�g�s�\�̏�폜���[�h��ON
	if (strcmp(fi[UNTEST_MODE].name, "ON") == 0) {
		////�����o�̏჊�X�g�̍쐬
		//�e�X�g�s�\�̏�֘A�t�@�C���o��
		file_output(0, 1);
		//�e�X�g�s�\�̏�폜
		if (untest_delete_main() != SUCCESS) {
			printf("�yERROR�z:Untest Fault Delete(mttg_main.c)\n");
			return ERROR;
		}
		//�e�X�g�s�\�̏�֘A�t�@�C���o��
		file_output(0, 0);
	}

	//�R���\�[���A�v���P�[�V�����̏�����
	system("cls");

	//�W���o��
	standard_output();

	//�e�X�g����
	while (ufl_num != 0) {
		//PBO�t�@�C��
		sprintf(txt_cmd, "%smttg_pbo.txt", folder_out);
		if ((fout_mttg_pbo = fopen(txt_cmd, "w")) == NULL) {
			printf("�yERROR�z:File open mttg_pbo.txt(mttg_main.c)\n");
			exit(1);
		}

		//�ڕW�̏�I��
		if (fault_select(unsat_flag) != SUCCESS) {
			printf("�yERROR�z:Fault Select(mttg_main.c)\n");
			return ERROR;
		}
		file_output(0, 3);

		//PBO�T�C�Y�L�q
		mttg_pbo_generate_min(fout_mttg_pbo);

		//�����H�̋L�q
		pbo_generate_normal_c(fout_mttg_pbo);

		suffix = n_net * 2 + 1;
		head = tfl;
		i = 0;
		while (head != NULL) {
			//�̏�͈͂̓���
			specific_fault_range(head->name);
			//�̏��H�̋L�q
			pbo_generate_fault_c(fout_mttg_pbo, head);
			//�̏ጟ�o��H�̋L�q
			pbo_generate_fdetect_c_mttg(fout_mttg_pbo, head);
			//���̌̏�
			head = head->next;
			i++;
		}
		printf("\n");
		
		//�̏��N
		head = tfl;
		i = 0;
		while (head != NULL) {
			if (i == 0) {
				pbo_generate_fault_ex(fout_mttg_pbo, head,0);
			}
			else {
				pbo_generate_fault_ex(fout_mttg_pbo, head, 1);
			}
			head = head->next;
			i++;
		}

		//�����d�͐���
		if (strcmp(fi[LOW_POWER].name, "ON") == 0) {
			pbo_generate_power_mttg(fout_mttg_pbo);
		}

		fclose(fout_mttg_pbo);

		//========================================================================
		//	clasp �� �e�X�g�x�N�g������
		//========================================================================
		//Clasp
		sprintf(cmd, "clasp.exe %smttg_pbo.txt --time-limit %d >%sclasp_log_mttg.txt",folder_out, fi[TIME].value,folder_out);
		system(cmd);
		//�e�X�g�x�N�g������
		sprintf(cmd, "generate_tv.exe %sclasp_log_mttg.txt %d",folder_out, n_pi + n_dff);
		system(cmd);

		//�����e�X�g�p�^�[���m�F(����d�͎w�����̂�)
		if ((mttg_check_wsa() != SAT) && (strcmp(fi[LOW_POWER].name, "ON") == 0)) {
			printf("�yERROR�z:MTTG TEST NG PATTERN(mttg_main.c)\n");
			return ERROR;
		}

		//�e�X�g�W���ɒǉ�
		make_test_set(fout_tp);

		//�_���V�~�����[�V����
		if (logic_sim() != SUCCESS) {
			printf("�yERROR�z:Logic Simulation(mttg_main.c)\n");
			return ERROR;
		}

		//========================================================================
		//	�̏�V�~�����[�V����
		//========================================================================
		sprintf(cmd, "XID2.exe -c %s -tx tv.txt -pin %spin.txt -fm TDF -fsim YES -dic %sfsim_log.txt -det %sxid_det.txt -undet %sxid_undet.txt", fi[CIRCUIT].name, folder_out, folder_out, folder_out,folder_out);
		system(cmd);

		//�R���\�[���A�v���P�[�V�����̏�����
		system("cls");

		//���o�̏����&���o�̏ᐔ�𖢌��o�̏Ⴉ��폜
		if ((det_fault_clasp_num=fault_judge()) == ERROR) {
			printf("�yERROR�z:Fault Judge(mttg_main.c)\n");
			return ERROR;
		}
		
		//��O�I������
		if (det_fault_clasp_num == 0) {
			unsat_flag = ON;
		}
		else unsat_flag = OFF;

	}

	fclose(fout_tp);

	return SUCCESS;
}
