#include"Netlist.h"

//------------------------------------------------------------------------
//�����o�̏�W��
//------------------------------------------------------------------------
typedef struct Undet_fault_list{
	int type;						   //�̏�^�C�v(STF:1->0 or STR:0->1)
	char name[256];					   //�̏�M������
	NLIST* netlist;	                   //�l�b�g���X�g
	int node_id;                       //�M����ID
	int tfl_det_suffix;			       //�ڕW�̏᎞�̌̏ጟ�o��H�̏o�͔ԍ�
	int tfl_suf_fc;                    //suf_fc�͖ڕW�̏Ⴒ�Ƃɕς��̂Ŋi�[
	int tfl_fc_ppo;					   //�̏��H�Ɋ܂܂��PPO���Z�o
	struct Undet_fault_list* next;	   //�㑱�A�h���X
}UNDET_FAULT_LIST;

//------------------------------------------------------------------------
//PBO�̃T�C�Y
//------------------------------------------------------------------------
typedef struct Size_Pbo{
	int val;		                   //�ϐ��̐�
	int h_constraint;                  //����
	int s_constraint;                  //�\�t�g����
	int lit;		                   //���e������
}PBO;
