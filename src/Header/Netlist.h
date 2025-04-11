//------------------------------------------------------------------------
//Netlist ��`, �O���ϐ�, �v���g�^�C�v�錾
//File name : Nelist.h
//Date : 2006/03/26
//Designer : R.Inoue
//Ver : 3.00�i�z�zVer�j
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// ��`
//------------------------------------------------------------------------
//�Q�[�g�^�C�v
#define  IN		0
#define  BUF    10
#define  INV	11
#define  AND	12
#define  NAND	13
#define  OR		14
#define  NOR	15
#define  EXOR	16 //2���͂̂݉�
#define  EXNOR	17 //2���͂̂݉�
#define  FOUT	18
#define  DFF	19 //D-�t���b�v�t���b�v
#define  RDFF	21 //D-�t���b�v�t���b�v�i���Z�b�g�L�j
#define  DFFS	23 //D-�X�L�����t���b�v�t���b�v
#define  RDFFS	24 //D-�X�L�����t���b�v�t���b�v�i���Z�b�g�L�j
#define  GND	25 //�O�����h
#define  ACC	26 //�d��

//�l�b�g���X�g�\���̒�`
typedef struct _Netlist_Format_ {
	char *name;						//���O
	int n;							//ID�i���j�[�N�j
	int type;						//�^�C�v
	int n_in;						//���͐�
	struct _Netlist_Format_ **in;	//���̓|�C���^�z��
	int n_out;						//�o�͐�
	struct _Netlist_Format_ **out;	//�o�̓|�C���^�z��
	char *name_port;				//�[�q�� pin��
	char *name_ins;					//�C���X�^���X��
	int level;						//���x���t��

	int open_pass;					//�I�[�v���p�X
	int fault_pass;					//�̏ᓞ�B�\�p�X

	int flag;					    //�t���O
	int ppo_flag;				    //�[���O���o�̓t���O
	int tf_flag;					//�ڕW�̏�t���O

	int val_1t;						//1�����ڂ̘_���l
	int val_2t;						//2�����ڂ̘_���l

	unsigned int suf_1t;			//1�����ڂ̓Y����
	unsigned int suf_2t;			//2�����ڂ̓Y����
	unsigned int suf_fc;			//�̏��H�̓Y����

} NLIST;

//------------------------------------------------------------------------
// �O���ϐ�
//------------------------------------------------------------------------
//�l�b�g���X�g
NLIST *nl;

//���W���[����
char *module_name;

//�M����
int n_net;

//�O�����͐�, �O������
NLIST **pi;
int n_pi;

//�^���O���o�͐�, �^���O���o��
NLIST **ppi;
int n_ppi;

//�O���o�͐�, �O���o��
NLIST **po;
int n_po;

//�^���O���o�͐�, �^���O���o��
NLIST **ppo;
int n_ppo;

//DFF, DFF��
NLIST **dff;
int n_dff;

//RDFF, RDFF��
NLIST **rdff;
int n_rdff;

//DFFS, DFFS��
NLIST **dffs;
int n_dffs;

//RDFFS, RDFFS��
NLIST **rdffs;
int n_rdffs;

//assign
NLIST **assign;
int n_assign;
