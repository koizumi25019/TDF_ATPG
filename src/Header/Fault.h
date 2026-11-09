#pragma once

#include"Netlist.h"

//------------------------------------------------------------------------
//未検出故障集合
//------------------------------------------------------------------------
typedef struct Undet_fault_list{
	int type;						   //故障タイプ(STF:1->0 or STR:0->1)
	char name[256];					   //故障信号線名
	NLIST* netlist;	                   //ネットリスト
	int node_id;                       //信号線ID
	int tfl_det_suffix;			       //目標故障時の故障検出回路の出力番号
	int tfl_suf_fc;                    //suf_fcは目標故障ごとに変わるので格納
	int tfl_fc_ppo;					   //故障回路に含まれるPPO数算出
	int is_target;					   //今回の目標故障か(1:目標故障)。検出された目標故障を数えるために使う
	int abort_reason;				   //未確定故障になった理由(UFL_ABORT_*。それ以外は0)
	struct Undet_fault_list* next;	   //後続アドレス
}UNDET_FAULT_LIST;

//------------------------------------------------------------------------
//PBOのサイズ
//------------------------------------------------------------------------
typedef struct Size_Pbo{
	int val;		                   //変数の数
	int h_constraint;                  //制約数
	int s_constraint;                  //ソフト制約数
	int lit;		                   //リテラル数
}PBO;
