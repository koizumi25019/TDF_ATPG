#pragma once

//------------------------------------------------------------------------
// グローバル変数の「定義」と「宣言」の切り替え
// Main/globals.c だけが DEFINE_GLOBALS を定義してから本ヘッダを読み込み、
// 変数の実体を作る。それ以外のファイルでは extern 宣言になる。
// (ヘッダ内で変数の実体を定義すると、.c ファイルの数だけ多重定義になるため)
//------------------------------------------------------------------------
#ifdef DEFINE_GLOBALS
#define GLOBAL
#else
#define GLOBAL extern
#endif

#include"Command_Info.h"
#include"Stack.h"
#include<time.h>
//------------------------------------------------------------------------
// 実行ステータス
//------------------------------------------------------------------------
#define		NO				0
#define		YES				1
#define     ERROR           -1
#define     FAILURE			-1
#define		SUCCESS			0	
#define		SAT			    1
#define		UNSAT			0


//------------------------------------------------------------------------
// フラグ
//------------------------------------------------------------------------
#define		ON				1
#define		OFF				0
#define		FLAG_DOWN		0
#define		FLAG_UP		    1


//------------------------------------------------------------------------
//　ファイル情報
//------------------------------------------------------------------------

/* 入力ファイル */
#define		CIRCUIT			0

/* オプション */
#define		RAND			1
#define		TIME			2
#define		UNTEST_MODE		3
#define		LOW_POWER		4
#define		WSATH			5
#define		TARGET_LIMIT	6
#define		FAULT	        7
#define     FOLDER          8

//------------------------------------------------------------------------
//故障のタイプ
//------------------------------------------------------------------------
#define		STF		    0		   //STF(1->0)
#define		STR			1          //STR(0->1)

//------------------------------------------------------------------------
//ソートテーブル
//------------------------------------------------------------------------
GLOBAL NLIST** sort_nl;
GLOBAL STACK Level;			//レベライズ用のスタック

//------------------------------------------------------------------------
//出力ファイル
//------------------------------------------------------------------------
GLOBAL FILE* fout_tp;			//テストパターン集合ファイル
GLOBAL FILE* fout_fault;		//故障集合ファイル
GLOBAL FILE* fout_pin_input;	//入力ピンファイル
GLOBAL FILE* fout_pin;			//入力ピンファイル(故障シミュレーション)
GLOBAL FILE* fout_log;			//ログファイル
GLOBAL FILE* fout_undet;		//最終未検出故障ファイル(空ファイル)
GLOBAL FILE* fout_det;			//最終検出故障ファイル(＝初期未検出故障集合ファイル)

GLOBAL FILE* fout_untest_tp;		//テスト不能故障削除時のテストパターン集合ファイル
GLOBAL FILE* fout_untest_fault;		//テスト不能故障集合ファイル
GLOBAL FILE* fout_high_fault;		//高消費電力故障集合ファイル
GLOBAL FILE* fout_init_undet;		//初期未検出集合ファイル
GLOBAL FILE* fout_untest_pbo;		//テスト不能故障削除用のSTTGのPBOファイル
GLOBAL FILE* fout_literal;			//テスト不能削除前のリテラル情報

GLOBAL FILE* fout_mttg_pbo;		//MTTGのPBOファイル
GLOBAL FILE* fout_fsim_result;	    //故障辞書ファイル
GLOBAL FILE* fout_mttg;	        //1TP当たりで検出された故障数ファイル
GLOBAL FILE* fout_tfl_log;         //目標故障集合ログファイル

//------------------------------------------------------------------------
//入力ファイル
//------------------------------------------------------------------------
GLOBAL FILE* fin_untest_pbo_result;	//テスト不能故障削除用のSTTGのPBO結果ファイル

//------------------------------------------------------------------------
//フォルダー指定								
//------------------------------------------------------------------------
GLOBAL char* folder_out;	//最終結果の出力フォルダー(末尾は必ず'/')
GLOBAL char* folder_tmp;	//中間生成ファイル用フォルダー(folder_out + "tmp/")。毎回上書きされる作業用ファイルだけを置く
GLOBAL char* tv_path;		//生成したテストベクトル(tv.txt)の置き場所。generate_tv.exe は実行フォルダに tv.txt を出すので、その後ここへ移す

//------------------------------------------------------------------------
//遷移故障用
//------------------------------------------------------------------------
GLOBAL int all_num;					//全故障数
GLOBAL UNDET_FAULT_LIST *afl;		//全故障リスト
GLOBAL int dfl_num;					//検出故障数
GLOBAL UNDET_FAULT_LIST *dfl;		//検出故障リスト
GLOBAL int ufl_num;					//未検出故障数
GLOBAL UNDET_FAULT_LIST *ufl;		//未検出故障リスト
GLOBAL int red_num;					//テスト不能故障数
GLOBAL UNDET_FAULT_LIST *red;		//テスト不能故障リスト
GLOBAL int hpfl_num;				//高消費電力故障数
GLOBAL UNDET_FAULT_LIST *hpfl;		//高消費電力故障リスト
GLOBAL int tfl_num;					//目標故障数
GLOBAL UNDET_FAULT_LIST* tfl;		//目標故障リスト



//------------------------------------------------------------------------
//その他							
//------------------------------------------------------------------------
#define		X	-1				//ドントケア
GLOBAL unsigned int suffix;			//添え字
GLOBAL STACK Open;					//オープンパス用のスタック
GLOBAL unsigned int fc_ppo;			//PPO数初期化
GLOBAL QUE Que;						//キュー
GLOBAL int tp_num;					//テストベクトル数

//------------------------------------------------------------------------
//プロトタイプ宣言
//------------------------------------------------------------------------
//Level_stack.c
void level_push(NLIST* p);
NLIST* level_pop(void);
//levelize.c
int levelize(void);
int judge_in_level(NLIST **in_pin, int n);
//Qsort.c
void Qsort(void);
int compare_int(const void *a, const void *b);
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
int generate_test_vector(const char* clasp_log);
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
void print_info(FILE* fp);
//check_open_pass.c
int check_open_pass(void);
//open_stack.c
void open_push(NLIST* p);
NLIST* open_pop(void);
//fault_select.c
int fault_select(int unsat_flag);
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
//mttg_calc_pbo_size.c
PBO mttg_calc_pbo_size(void);
PBO mttg_calc_hard_constraint(void);
int mttg_calc_soft_constraint(void);
//mttg_check_wsa.c
int check_wsa(const char* high_name, const char* low_name);
int mttg_check_wsa(void);
//mttg_main.c
int mttg_main(void);
//mttg_pbo_generate.c
void mttg_pbo_generate_min(FILE* fp);
void pbo_generate_fdetect_circuit(FILE *fp, UNDET_FAULT_LIST* node);
void pbo_generate_fault_ex(FILE *fp, UNDET_FAULT_LIST* node, int mode);
//specific_fault_range.c
int specific_fault_range(NLIST* fault_net);
//undet_fault_delete.c
void ufl_remove(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node);
void ufl_free(UNDET_FAULT_LIST* node, UNDET_FAULT_LIST* pre_node,int mode);
