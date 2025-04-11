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
//  �֐��� : mttg_pbo_generate_min
//  �@  �\ : MTTG�pPBO�t�@�C���L�q�y�œK���֐��E�T�C�Y(variable�Econstraint�E���̑�)�z
//  �߂�l : �Ȃ�
//  ��  �� : �t�@�C���|�C���^
//=========================================================================
void mttg_pbo_generate_min(FILE* fp) {

	PBO size_pbo;
	int i;

	//---------------------------------------------
	//PBO�T�C�Y�v�Z
	//---------------------------------------------
	size_pbo = mttg_calc_pbo_size();

	//-----------------------------------------------------------------------------
	//PBO�t�@�C���L�q
	//-----------------------------------------------------------------------------
	if (strcmp(fi[LOW_POWER].name, "OFF") == 0) {
		fprintf(fp, "* #variable= %d ", size_pbo.val + 1);
		fprintf(fp, "#constraint= %d\n", size_pbo.h_constraint);
		fprintf(fp, "#product= %d sizeproduct= %d\n", 2 * size_pbo.s_constraint, 4 * size_pbo.s_constraint);
		fprintf(fp, "*\n");
		fprintf(fp, "*  this is a dummy instance\n");
		fprintf(fp, "*\n");
	}
	else if (strcmp(fi[LOW_POWER].name, "ON") == 0) {
		fprintf(fp, "* #variable= %d ", size_pbo.val);
		fprintf(fp, "#constraint= %d ", size_pbo.h_constraint);
		fprintf(fp, "#product= %d sizeproduct= %d\n", 2 * size_pbo.s_constraint, 4 * size_pbo.s_constraint);
		fprintf(fp, "*\n");
		fprintf(fp, "*  this is a dummy instance\n");
		fprintf(fp, "*\n");
	}

	fprintf(fp, "min: 1 x%d", size_pbo.val - size_pbo.s_constraint);
	for (i = 2; i <= tfl_num; i++) {
		fprintf(fp, " +1 x%d", size_pbo.val - size_pbo.s_constraint + i - 1);
	}
	fprintf(fp, ";\n");

}

//=========================================================================
//  �֐��� : pbo_generate_fdetect_c_mttg
//  �@  �\ : �̏ጟ�o��H�L�q(MTTG����)
//  �߂�l : �Ȃ�
//  ��  �� : �t�@�C���|�C���^�C�Ώی̏�
//=========================================================================
void pbo_generate_fdetect_c_mttg(FILE *fp, UNDET_FAULT_LIST* node) {

	int i, j/*, k*/;
	unsigned int *fdetct_in;

	if ((fdetct_in = (unsigned int*)calloc(fc_ppo, sizeof(unsigned int))) == NULL) {
		printf("�yERROR�z:No calloc Fault Det Circuit(sttg_pbo_generate.c)\n"); //�̏ጟ�o��H�̓��͔z��̈�m�ێ��s
		//exit(1);
		abort();
	}

	node->tfl_fc_ppo = fc_ppo;
	node->tfl_suf_fc = node->netlist->suf_fc;

	j = 0;
	for (i = 0; i < n_net; i++) {
		if (nl[i].fault_pass == YES && nl[i].ppo_flag == 1 ) {
			//��EXOR
			fprintf(fp, "1 ~x%d +1 ~x%d +1 ~x%d >=1;\n", nl[i].suf_2t, nl[i].suf_fc, suffix);
			//nl[i].suf_2t�F�����2�����ڂ̏o��,nl[i].suf_fc:�̏��H�̏o��
			fprintf(fp, "1 x%d +1 x%d +1 ~x%d >=1;\n", nl[i].suf_2t, nl[i].suf_fc, suffix);
			fprintf(fp, "1 x%d +1 ~x%d +1 x%d >=1;\n", nl[i].suf_2t, nl[i].suf_fc, suffix);
			fprintf(fp, "1 ~x%d +1 x%d +1 x%d >=1;\n", nl[i].suf_2t, nl[i].suf_fc, suffix);
			if (fc_ppo == 1) {
				node->tfl_det_suffix = suffix;
			}
			fdetct_in[j] = suffix;
			suffix++;
			j++;
		}
	}

	if (fc_ppo != 1) {//fc_ppo�܂�̏��H�̊O���o�͂̐��������̏ꍇ���񎮂�ǉ��B�ǉ�����̂�OR�Q�[�g
		for (i = 0; (unsigned int)i < fc_ppo; i++) { fprintf(fp, "1 ~x%d +1 x%d >=1;\n", fdetct_in[i], suffix); }
		for (i = 0; (unsigned int)i < fc_ppo; i++) { fprintf(fp, "1 x%d +", fdetct_in[i]); }
		fprintf(fp, "1 ~x%d >=1;\n", suffix);
		node->tfl_det_suffix = suffix;
		suffix++;
	}
}

//=========================================================================
//  �֐��� : pbo_generate_fault_ex
//  �@  �\ : �̏��N�L�q(MTTG����)
//  �߂�l : �Ȃ�
//  ��  �� : �t�@�C���|�C���^�C�Ώی̏�,�n�[�h(mode=0),�\�t�g(mode=1)
//=========================================================================
void pbo_generate_fault_ex(FILE *fp, UNDET_FAULT_LIST* node,int mode) {

	//int i;


	//�ڕW�̏�1��(�n�[�h)
	if(mode == 0) {
		fprintf(fp, "1 x%d >=1;\n", node->tfl_det_suffix);
		//�̏ጟ�o��H�ɂ�����ŏI�o�͐����ӂɔ��f���邽�߂̕ϐ�
		//�̏Ⴊ���o������H��ł̈ʒu������
		switch (node->type) {
		case STF:
			fprintf(fp, "1 x%d >=1;\n", node->netlist->suf_1t);
			fprintf(fp, "1 ~x%d >=1;\n", node->netlist->suf_2t);
			fprintf(fp, "1 x%d >=1;\n", node->tfl_suf_fc);
			break;
		case STR:
			fprintf(fp, "1 ~x%d >=1;\n", node->netlist->suf_1t);
			fprintf(fp, "1 x%d >=1;\n", node->netlist->suf_2t);
			fprintf(fp, "1 ~x%d >=1;\n", node->tfl_suf_fc);
			break;
		}
	}

	//�ڕW�̏�1�ڈȊO(�\�t�g)
	else if (mode == 1) {
		fprintf(fp, "1 x%d +1 x%d >=1;\n", suffix, node->tfl_det_suffix);
		switch (node->type) {
		case STF:
			fprintf(fp, "1 x%d +1 x%d >=1;\n", suffix, node->netlist->suf_1t);
			fprintf(fp, "1 x%d +1 ~x%d >=1;\n", suffix, node->netlist->suf_2t);
			fprintf(fp, "1 x%d +1 x%d >=1;\n", suffix,node->tfl_suf_fc);
			break;
		case STR:
			fprintf(fp, "1 x%d +1 ~x%d >=1;\n", suffix,node->netlist->suf_1t);
			fprintf(fp, "1 x%d +1 x%d >=1;\n", suffix,node->netlist->suf_2t);
			fprintf(fp, "1 x%d +1 ~x%d >=1;\n", suffix, node->tfl_suf_fc);
			break;
		}
		suffix++;
	}

}


//=========================================================================
//  �֐��� : pbo_generate_power_mttg
//  �@  �\ : ����d�͐���(MTTG)(�\�t�g)
//  �߂�l : �Ȃ�
//  ��  �� : �t�@�C���|�C���^
//=========================================================================
void pbo_generate_power_mttg(FILE *fp) {
	int i;
	char* cons1 = (char*)NULL;

	if ((cons1 = (char*)calloc(500000, sizeof(char))) == NULL) {
		printf("�yERROR�z:No calloc cons1(mttg_pbo_generate.c)\n");
		exit(1);
	}

	//�M�����J�ڐ���
	for (i = 0; i < n_net; i++) {
		if (nl[i].open_pass == NO) {

			fprintf(fp, "1 x%d +1 x%d x%d +1 ~x%d ~x%d >=1;\n", suffix, nl[i].suf_1t, nl[i].suf_2t, nl[i].suf_1t, nl[i].suf_2t);
			
			//WSA����
			sprintf(cons1,"%s-1 x%d ", cons1, suffix);
			suffix++;
		}
	}

	//WSA����(WSA臒l)
	sprintf(cons1,  "%s>=-%d;\n",cons1,n_net *  fi[WSATH].value / 100);
	suffix++;

	fprintf(fp, "%s", cons1);

}