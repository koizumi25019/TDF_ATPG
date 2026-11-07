#pragma once

//------------------------------------------------------------------------
//Netlist 定義, 外部変数, プロトタイプ宣言
//File name : Nelist.h
//Date : 2006/03/26
//Designer : R.Inoue
//Ver : 3.00（配布Ver）
//------------------------------------------------------------------------
//------------------------------------------------------------------------
// 定義
//------------------------------------------------------------------------
//ゲートタイプ
#define  IN		0
#define  BUF    10
#define  INV	11
#define  AND	12
#define  NAND	13
#define  OR		14
#define  NOR	15
#define  EXOR	16 //2入力のみ可
#define  EXNOR	17 //2入力のみ可
#define  FOUT	18
#define  DFF	19 //D-フリップフロップ
#define  RDFF	21 //D-フリップフロップ（リセット有）
#define  DFFS	23 //D-スキャンフリップフロップ
#define  RDFFS	24 //D-スキャンフリップフロップ（リセット有）
#define  GND	25 //グランド
#define  ACC	26 //電源

//ネットリスト構造体定義
typedef struct _Netlist_Format_ {
	char *name;						//名前
	int n;							//ID（ユニーク）
	int type;						//タイプ
	int n_in;						//入力数
	struct _Netlist_Format_ **in;	//入力ポインタ配列
	int n_out;						//出力数
	struct _Netlist_Format_ **out;	//出力ポインタ配列
	char *name_port;				//端子名 pin名
	char *name_ins;					//インスタンス名
	int level;						//レベル付け

	int open_pass;					//オープンパス
	int fault_pass;					//故障到達可能パス

	int flag;					    //フラグ
	int ppo_flag;				    //擬似外部出力フラグ
	int tf_flag;					//目標故障フラグ

	int val_1t;						//1時刻目の論理値
	int val_2t;						//2時刻目の論理値

	unsigned int suf_1t;			//1時刻目の添え字
	unsigned int suf_2t;			//2時刻目の添え字
	unsigned int suf_fc;			//故障回路の添え字

} NLIST;

//------------------------------------------------------------------------
// 外部変数
//------------------------------------------------------------------------
//ネットリスト
GLOBAL NLIST *nl;

//モジュール名
GLOBAL char *module_name;

//信号数
GLOBAL int n_net;

//外部入力数, 外部入力
GLOBAL NLIST **pi;
GLOBAL int n_pi;

//疑似外部出力数, 疑似外部出力
GLOBAL NLIST **ppi;
GLOBAL int n_ppi;

//外部出力数, 外部出力
GLOBAL NLIST **po;
GLOBAL int n_po;

//疑似外部出力数, 疑似外部出力
GLOBAL NLIST **ppo;
GLOBAL int n_ppo;

//DFF, DFF数
GLOBAL NLIST **dff;
GLOBAL int n_dff;

//RDFF, RDFF数
GLOBAL NLIST **rdff;
GLOBAL int n_rdff;

//DFFS, DFFS数
GLOBAL NLIST **dffs;
GLOBAL int n_dffs;

//RDFFS, RDFFS数
GLOBAL NLIST **rdffs;
GLOBAL int n_rdffs;

//assign
GLOBAL NLIST **assign;
GLOBAL int n_assign;
