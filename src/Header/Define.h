#pragma once

#include"Command_Info.h"
#include"Stack.h"
#include<time.h>
//------------------------------------------------------------------------
// ���s�X�e�[�^�X
//------------------------------------------------------------------------
#define		NO				0
#define		YES				1
#define     ERROR           -1
#define     FAILURE			-1
#define		SUCCESS			0	
#define		SAT			    1
#define		UNSAT			0


//------------------------------------------------------------------------
// �t���O
//------------------------------------------------------------------------
#define		ON				1
#define		OFF				0
#define		FLAG_DOWN		0
#define		FLAG_UP		    1


//------------------------------------------------------------------------
//�@�t�@�C�����
//------------------------------------------------------------------------

/* ���̓t�@�C�� */
#define		CIRCUIT			0

/* �I�v�V���� */
#define		RAND			1
#define		TIME			2
#define		UNTEST_MODE		3
#define		LOW_POWER		4
#define		WSATH			5
#define		TARGET_LIMIT	6
#define		FAULT	        7
#define     FOLDER          8

//------------------------------------------------------------------------
//all0 all1
//------------------------------------------------------------------------
#define		all1		4294967295	//�_���l1�̂Ƃ��̒l
#define		all0		0			//�_���l0�̂Ƃ��̒l

//------------------------------------------------------------------------
//�̏�̃^�C�v
//------------------------------------------------------------------------
#define		STF		    0		   //STF(1->0)
#define		STR			1          //STR(0->1)

//------------------------------------------------------------------------
//�\�[�g�e�[�u��
//------------------------------------------------------------------------
NLIST** sort_nl;
STACK Level;			//���x���C�Y�p�̃X�^�b�N

//------------------------------------------------------------------------
//�o�̓t�@�C��
//------------------------------------------------------------------------
FILE* fout_tp;			//�e�X�g�p�^�[���W���t�@�C��
FILE* fout_fault;		//�̏�W���t�@�C��
FILE* fout_pin_input;	//���̓s���t�@�C��
FILE* fout_pin;			//���̓s���t�@�C��(�̏�V�~�����[�V����)
FILE* fout_log;			//���O�t�@�C��
FILE* fout_undet;		//�ŏI�����o�̏�t�@�C��(��t�@�C��)
FILE* fout_det;			//�ŏI���o�̏�t�@�C��(�����������o�̏�W���t�@�C��)

FILE* fout_untest_tp;		//�e�X�g�s�\�̏�폜���̃e�X�g�p�^�[���W���t�@�C��
FILE* fout_untest_fault;	//�e�X�g�s�\�̏�W���t�@�C��
FILE* fout_high_fault;		//������d�͌̏�W���t�@�C��
FILE* fout_init_undet;		//���������o�W���t�@�C��
FILE* fout_untest_pbo;		//�e�X�g�s�\�̏�폜�p��STTG��PBO�t�@�C��
FILE* fout_literal;			//�e�X�g�s�\�폜�O�̃��e�������

FILE* fout_mttg_pbo;		//MTTG��PBO�t�@�C��
FILE* fout_mini_pbo;		//�ڕW�̏�I������miniPBO
FILE* fout_mini_pbo_copy;	//�ڕW�̏�I������1�O�̃t�@�C���ۑ��p
FILE* fout_fsim_result;	    //�̏Ꭻ���t�@�C��
FILE* fout_mttg;	        //1TP������Ō��o���ꂽ�̏ᐔ�t�@�C��
FILE* fout_log;				//���O�t�@�C��
FILE* fout_tfl_log;         //�ڕW�̏�W�����O�t�@�C��

//------------------------------------------------------------------------
//���̓t�@�C��
//------------------------------------------------------------------------
FILE* fin_untest_pbo_result;	//�e�X�g�s�\�̏�폜�p��STTG��PBO���ʃt�@�C��
FILE* fin_mini_pbo_result;	    //�ڕW�̏�I��p��PBO���ʃt�@�C��

//------------------------------------------------------------------------
//�t�H���_�[�w��								
//------------------------------------------------------------------------
char* folder_out;		//�o�̓t�@�C���p�̃t�H���_�[��

//------------------------------------------------------------------------
//�J�ڌ̏�p
//------------------------------------------------------------------------
int all_num;					//�S�̏ᐔ
UNDET_FAULT_LIST *afl;			//�S�̏჊�X�g
int dfl_num;					//���o�̏ᐔ
UNDET_FAULT_LIST *dfl;			//���o�̏჊�X�g
int ufl_num;					//�����o�̏ᐔ
UNDET_FAULT_LIST *ufl;			//�����o�̏჊�X�g
int red_num;					//�e�X�g�s�\�̏ᐔ
UNDET_FAULT_LIST *red;			//�e�X�g�s�\�̏჊�X�g
int hpfl_num;					//������d�͌̏ᐔ
UNDET_FAULT_LIST *hpfl;			//������d�͌̏჊�X�g
int node_num;                   //���̏�M������
int tfl_num;					//�ڕW�̏ᐔ
UNDET_FAULT_LIST* tfl;			//�ڕW�̏჊�X�g
UNDET_FAULT_LIST** tfl_node;	//�ڕW�̏�̐M�����W��



//------------------------------------------------------------------------
//���̑�							
//------------------------------------------------------------------------
#define		X	-1				//�h���g�P�A
unsigned int suffix;			//�Y����
STACK Open;						//�I�[�v���p�X�p�̃X�^�b�N
unsigned int fc_ppo;			//PPO��������
QUE Que;						//�L���[
int tp_num;						//�e�X�g�x�N�g����

//------------------------------------------------------------------------
//�v���g�^�C�v�錾
//------------------------------------------------------------------------
//Level_stack.c
void level_push(NLIST* p);
NLIST* level_pop(void);
//levelize.c
int levelize(void);
int judge_in_level(NLIST **in_pin, int n);
//Qsort.c
void Qsort(void);
int compare_int(void *a, void *b);
//set_literal_id.c
void set_literal_id(void);
//cmdline.c
int cmdline(int argc, char *argv[]);
//init_exturn.c
int init_exturn(void);
//make_ppo.c
void make_ppo_ppi(void);
//Netlist.c
void read_nl(char *name);
//and.c
int and(NLIST *net, int flag);
//fout.c
int fout(NLIST *net, int flag);
//inv.c
int inv(NLIST *net, int flag);
//nand.c
int nand(NLIST *net, int flag);
//nor.c
int nor(NLIST *net, int flag);
//or.c
int or (NLIST *net, int flag);
//logic_sim.c
int logic_sim(void);
//logic_sim_1time.c
int logic_sim_1time(void);
//logic_sim_2time.c
int logic_sim_2time(void);
//ppo_ppi_cat.c
int ppo_ppi_cat(void);
//test_v_read.c
int test_v_read(void);
//fault_judge.c
int fault_judge(void);
//file_output.c
int file_output(clock_t time, int mode);
//make_test_set.c
void make_test_set(FILE *fp_ts);
//pin_make.c
int pin_make(void);
//Que.c
void enqueue(NLIST *data, int mode);
NLIST* dequeue(void);
int queue_emp(void);
//reset_flag.c
void reset_flag(void);
//standard_output.c
void standard_output(void);
//check_open_pass.c
int check_open_pass(void);
//open_stack.c
void open_push(NLIST* p);
NLIST* open_pop(void);
//fault_select.c
int fault_select(int unsat_flag);
//mini_calc_pbo_size.c
PBO mini_calc_pbo_size(void);
int mini_calc_hard_constraint(void);
//mini_pbo_generate.c
void mini_pbo_generate_min(FILE* fp);
//untest_calc_pbo_size.c
PBO untest_calc_pbo_size(UNDET_FAULT_LIST* node);
int untest_calc_hard_constraint(UNDET_FAULT_LIST* node);
int untest_calc_soft_constraint(void);
//untest_check_wsa.c
int untest_check_wsa(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node);
//untest_pbo_generation.c
void untest_pbo_generate_min(FILE* fp, UNDET_FAULT_LIST* node);
void pbo_generate_normal_c(FILE *fp);
void pbo_generate_fault_c(FILE *fp, UNDET_FAULT_LIST* node);
void pbo_generate_fdetect_c(FILE *fp, UNDET_FAULT_LIST* node);
void pbo_generate_power(FILE *fp);
//unsat_dicision.c
int unsat_dicision(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node,int mode);
//untest_delete_main.c
int untest_delete_main(void);
//transition_fault_set.c
int input_transition_delay_fault_set(void);
int all_transition_delay_fault_set(void);
NLIST* search_netlist(char* name);
//fault_select.c
int fault_select(int unsat_flag);
//mttg_calc_pbo_size.c
PBO mttg_calc_pbo_size(void);
PBO mttg_calc_hard_constraint(void);
int mttg_calc_soft_constraint(void);
//mttg_check_wsa.c
int mttg_check_wsa(void);
//mttg_main.c
int mttg_main(void);
//mttg_pbo_generate.c
void mttg_pbo_generate_min(FILE* fp);
void pbo_generate_fdetect_c_mttg(FILE *fp, UNDET_FAULT_LIST* node);
void pbo_generate_fault_ex(FILE *fp, UNDET_FAULT_LIST* node, int mode);
void pbo_generate_power_mttg(FILE *fp);
//specific_fault_range.c
int specific_fault_range(char* name);
//undet_fault_delete.c
int undet_fault_delete(UNDET_FAULT_LIST* node);
void ufl_free(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node,int mode);