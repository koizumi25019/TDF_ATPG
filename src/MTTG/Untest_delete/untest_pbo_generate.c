//------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include<stdlib.h>
#include<string.h>

//------------------------------------------------------------------------
//自作インクルード
//------------------------------------------------------------------------
#include"../../Header/Define.h"

//=========================================================================
//  関数名 : untest_pbo_generate_min
//  機  能 : PBOファイル記述【最適化関数・サイズ(variable・constraint・その他)】
//  戻り値 : なし
//  引  数 : ファイルポインタ,対象故障
//=========================================================================
void untest_pbo_generate_min(FILE* fp,UNDET_FAULT_LIST* node) {

	PBO size_pbo;
	//int i;

	//---------------------------------------------
	//PBOサイズ計算
	//---------------------------------------------
	size_pbo = untest_calc_pbo_size(node);

	//-----------------------------------------------------------------------------
	//PBOファイル記述
	//-----------------------------------------------------------------------------

	
	fprintf(fp, "* #variable= %d ", size_pbo.val);
	fprintf(fp, "#constraint= %d ", size_pbo.h_constraint);
	fprintf(fp, "#product= %d sizeproduct= %d\n", 2 * size_pbo.s_constraint, 4 * size_pbo.s_constraint);
	fprintf(fp, "*\n");
	fprintf(fp, "*  this is a dummy instance\n");
	fprintf(fp, "*\n");
	
	//---------------------------------------------
	//	PBOファイル書き込み（最適化関数）
	//---------------------------------------------
	fprintf(fp, "min: 1 x%d;\n", size_pbo.val);
}


//------------------------------------------------------------------------
//どの時刻(回路)の変数番号を使うか
//------------------------------------------------------------------------
#define FRAME_1T	0	//正常回路の1時刻目
#define FRAME_2T	1	//正常回路の2時刻目
#define FRAME_FC	2	//故障回路

//=========================================================================
//  関数名 : lit_of
//  機  能 : 信号線の、指定した時刻(回路)での変数番号を返す
//  戻り値 : 変数番号
//  引  数 : 信号線，時刻(回路)
//=========================================================================
static unsigned int lit_of(const NLIST* net, int frame) {
	switch (frame) {
	case FRAME_1T: return net->suf_1t;
	case FRAME_2T: return net->suf_2t;
	default:       return net->suf_fc;
	}
}

//=========================================================================
//  関数名 : emit_gate
//  機  能 : 論理ゲート1つ分の制約(CNF)を、指定した時刻(回路)の変数番号で書き出す
//           (正常回路の1時刻目/2時刻目、故障回路で同じ式を使い回すための関数)
//  戻り値 : なし
//  引  数 : ファイルポインタ，ゲート，時刻(回路)
//=========================================================================
static void emit_gate(FILE* fp, const NLIST* g, int frame) {
	int j;
	unsigned int o = lit_of(g, frame);					//出力
	unsigned int in0 = 0, in1 = 0;						//入力(INV/BUF/FOUT/EXORで使用)

	if (g->n_in >= 1) in0 = lit_of(g->in[0], frame);
	if (g->n_in >= 2) in1 = lit_of(g->in[1], frame);

	switch (g->type) {
		//●AND
	case AND:
		for (j = 0; j < g->n_in; j++) { fprintf(fp, "1 x%u +1 ~x%u >=1;\n", lit_of(g->in[j], frame), o); }
		for (j = 0; j < g->n_in; j++) { fprintf(fp, "1 ~x%u +", lit_of(g->in[j], frame)); }
		fprintf(fp, "1 x%u >=1;\n", o);
		break;
		//●NAND
	case NAND:
		for (j = 0; j < g->n_in; j++) { fprintf(fp, "1 x%u +1 x%u >=1;\n", lit_of(g->in[j], frame), o); }
		for (j = 0; j < g->n_in; j++) { fprintf(fp, "1 ~x%u +", lit_of(g->in[j], frame)); }
		fprintf(fp, "1 ~x%u >=1;\n", o);
		break;
		//●OR
	case OR:
		for (j = 0; j < g->n_in; j++) { fprintf(fp, "1 ~x%u +1 x%u >=1;\n", lit_of(g->in[j], frame), o); }
		for (j = 0; j < g->n_in; j++) { fprintf(fp, "1 x%u +", lit_of(g->in[j], frame)); }
		fprintf(fp, "1 ~x%u >=1;\n", o);
		break;
		//●NOR
	case NOR:
		for (j = 0; j < g->n_in; j++) { fprintf(fp, "1 ~x%u +1 ~x%u >=1;\n", lit_of(g->in[j], frame), o); }
		for (j = 0; j < g->n_in; j++) { fprintf(fp, "1 x%u +", lit_of(g->in[j], frame)); }
		fprintf(fp, "1 x%u >=1;\n", o);
		break;
		//●INV
	case INV:
		fprintf(fp, "1 ~x%u +1 ~x%u >=1;\n1 x%u +1 x%u >=1;\n", in0, o, in0, o);
		break;
		//●FOUT, BUF
	case FOUT:
	case BUF:
		fprintf(fp, "1 ~x%u +1 x%u >=1;\n1 x%u +1 ~x%u >=1;\n", in0, o, in0, o);
		break;
		//●EXOR
	case EXOR:
		fprintf(fp, "1 ~x%u +1 ~x%u +1 ~x%u >=1;\n", in0, in1, o);
		fprintf(fp, "1 x%u +1 x%u +1 ~x%u >=1;\n", in0, in1, o);
		fprintf(fp, "1 x%u +1 ~x%u +1 x%u >=1;\n", in0, in1, o);
		fprintf(fp, "1 ~x%u +1 x%u +1 x%u >=1;\n", in0, in1, o);
		break;
	}
}

//=========================================================================
//  関数名 : pbo_generate_normal_c
//  機  能 : 正常回路記述
//  戻り値 : なし
//  引  数 : ファイルポインタ
//=========================================================================
void pbo_generate_normal_c(FILE *fp) {

	int i;

	for (i = 0; i < n_net; i++) {
		switch (sort_nl[i]->type) {
			//●DFF(1時刻目のD入力 = 2時刻目のQ)
		case DFF:
			fprintf(fp, "1 ~x%u +1 x%u >=1;\n1 x%u +1 ~x%u >=1;\n", sort_nl[i]->in[0]->suf_1t, sort_nl[i]->suf_2t, sort_nl[i]->in[0]->suf_1t, sort_nl[i]->suf_2t);
			break;
			//●IN(外部入力は1時刻目と2時刻目で同じ値)
		case IN:
			fprintf(fp, "1 ~x%u +1 x%u >=1;\n1 x%u +1 ~x%u >=1;\n", sort_nl[i]->suf_1t, sort_nl[i]->suf_2t, sort_nl[i]->suf_1t, sort_nl[i]->suf_2t);
			break;
			//●論理ゲート(1時刻目、2時刻目の順)
		default:
			emit_gate(fp, sort_nl[i], FRAME_1T);
			emit_gate(fp, sort_nl[i], FRAME_2T);
			break;
		}
	}

}

//=========================================================================
//  関数名 : pbo_generate_fault_c
//  機  能 : 故障回路記述
//  戻り値 : なし
//  引  数 : ファイルポインタ,対象故障
//=========================================================================
void pbo_generate_fault_c(FILE *fp, UNDET_FAULT_LIST* node) {

	int i;

	for (i = 0; i < n_net; i++) {
		//故障信号線そのものは、故障励起の制約で値を決めるので式は作らない
		if (sort_nl[i]->fault_pass == YES && sort_nl[i] != node->netlist) {
			emit_gate(fp, sort_nl[i], FRAME_FC);
		}
	}

}


//=========================================================================
//  関数名 : pbo_generate_fdetect_c
//  機  能 : 故障検出回路記述(テスト不能故障削除用)
//           検出回路 ＋ 「検出されること」＋ 故障励起
//  戻り値 : なし
//  引  数 : ファイルポインタ，対象故障
//=========================================================================
void pbo_generate_fdetect_c(FILE *fp, UNDET_FAULT_LIST* node) {

	//検出回路(出力の変数番号は node->tfl_det_suffix に入る)
	pbo_generate_fdetect_circuit(fp, node);

	//検出されること ＋ 故障励起(mode=0:ハード制約)
	pbo_generate_fault_ex(fp, node, 0);

}

//=========================================================================
//  関数名 : pbo_generate_power
//  機  能 : 消費電力制約(テスト不能故障削除用・MTTG用 共通)
//  戻り値 : なし
//  引  数 : ファイルポインタ
//=========================================================================
void pbo_generate_power(FILE *fp) {
	int i;
	char* cons1 = (char*)NULL;

	if ((cons1 = (char*)calloc(500000, sizeof(char))) == NULL) {
		printf("【ERROR】:No calloc cons1(untest_pbo_generate.c)\n");
		exit(1);
	}

	for (i = 0; i < n_net; i++) {
		if (nl[i].open_pass == NO) {
			fprintf(fp, "1 x%d +1 x%d x%d +1 ~x%d ~x%d >=1;\n", suffix, nl[i].suf_1t, nl[i].suf_2t, nl[i].suf_1t, nl[i].suf_2t);
			//WSA制約
			sprintf(cons1, "%s-1 x%d ", cons1, suffix);
			suffix++;
		}
	}

	//WSA制約(WSA閾値)
	sprintf(cons1, "%s>=-%d;\n", cons1, n_net *  fi[WSATH].value / 100);
	suffix++;

	fprintf(fp, "%s", cons1);

	free(cons1);
}
