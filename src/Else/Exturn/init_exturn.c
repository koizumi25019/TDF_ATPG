//------------------------------------------------------------------------
//インクルード
//------------------------------------------------------------------------
#include <stdio.h>
#include <time.h>
#include<stdlib.h>
#include<string.h>
#include<direct.h>

//------------------------------------------------------------------------
//自作インクルード
//------------------------------------------------------------------------
#include"../../Header/Define.h"

//===================================================
//関数名:init_exturn
//機能:初期化関連
//戻り値：int 正常終了:SUCCESS(=0) , 異常終了:ERROR(=-1)
//引数：なし
//===================================================
int init_exturn(void) {
	char txt_cmd[1024];
	char* base;
	size_t len;

	//遷移故障関連
	red = NULL;
	hpfl = NULL;
	dfl = NULL;
	ufl_num = 0;		       //未検出故障数
	dfl_num = 0;			   //検出故障数
	red_num = 0;		       //テスト不能故障数
	hpfl_num = 0;			   //高消費電力故障数

	//その他
	tp_num = 0;					//テストベクトル数

	//キュー
	//(キューに入るのは故障回路に含まれる信号線で、高々 n_net 個。
	// 空判定が front==rear なので、満杯と空を区別するため +1 しておく)
	Que.max = n_net + 1;		//最大値
	Que.front = 0;				//先頭インデックス
	Que.rear = 0;				//末尾インデックス
	Que.num = 0;				//要素数
	if ((Que.que = (NLIST**)calloc(Que.max, sizeof(NLIST*))) == NULL) {
		printf("【ERROR】:No calloc Que(init_exturn.c)\n");
		return ERROR;
	}

	//レベライズ(スタックは足りなくなったら level_push が自動で拡張する)
	Level.max_num = n_net;			//初期容量
	Level.ptr = 0;					//インデックス
	if ((Level.stk = (NLIST**)calloc(Level.max_num, sizeof(NLIST*))) == NULL) {
		printf("【ERROR】:No calloc Level stack(init_exturn.c)\n");
		return ERROR;
	}

	//オープンパス(同上)
	Open.max_num = n_net;			//初期容量
	Open.ptr = 0;					//インデックス
	if ((Open.stk = (NLIST**)calloc(Open.max_num, sizeof(NLIST*))) == NULL) {
		printf("【ERROR】:No calloc Open stack(init_exturn.c)\n");
		return ERROR;
	}

	//フォルダー名の整理
	//末尾の '/' '\' をいくつあっても取り除き、folder_out は必ず '/' で終わる形にする
	//(指定が "./out" でも "./out/" でも同じに扱う)
	len = strlen(fi[FOLDER].name);
	if ((base = (char*)calloc(len + 1, sizeof(char))) == NULL) {
		printf("【ERROR】:No calloc Folder name(init_exturn.c)\n");
		return ERROR;
	}
	strcpy(base, fi[FOLDER].name);
	while (len > 0 && (base[len - 1] == '/' || base[len - 1] == '\\')) {
		base[--len] = '\0';
	}
	if (len == 0) {
		printf("【ERROR】:Invalid folder name(init_exturn.c)\n");
		free(base);
		return ERROR;
	}

	folder_out = (char*)calloc(len + 2, sizeof(char));		// "<base>/"
	folder_tmp = (char*)calloc(len + 6, sizeof(char));		// "<base>/tmp/"
	tv_path = (char*)calloc(len + 14, sizeof(char));		// "<base>/tmp/tv.txt"
	if (folder_out == NULL || folder_tmp == NULL || tv_path == NULL) {
		printf("【ERROR】:No calloc Folder name output(init_exturn.c)\n");
		free(base);
		return ERROR;
	}
	sprintf(folder_out, "%s/", base);
	sprintf(folder_tmp, "%s/tmp/", base);
	sprintf(tv_path, "%s/tmp/tv.txt", base);

	//フォルダーの作成(最終結果用 と 中間生成ファイル用)
	if (_mkdir(base) != 0) {
		printf("【ERROR】:Please Delete Same name Folder, or check the parent folder(init_exturn.c)\n");
		free(base);
		return ERROR;
	}
	snprintf(txt_cmd, sizeof(txt_cmd), "%s/tmp", base);
	if (_mkdir(txt_cmd) != 0) {
		printf("【ERROR】:Cannot make tmp Folder(init_exturn.c)\n");
		free(base);
		return ERROR;
	}
	free(base);

	//ファイルの初期化
	snprintf(txt_cmd, sizeof(txt_cmd), "%smttg.txt", folder_out);
	if ((fout_mttg = fopen(txt_cmd, "w")) == NULL) {
		printf("【ERROR】:File open mttg.txt(init_exturn.c)\n");
		return ERROR;
	}
	fclose(fout_mttg);

	snprintf(txt_cmd, sizeof(txt_cmd), "%stfl_log.txt", folder_out);
	if ((fout_tfl_log = fopen(txt_cmd, "w")) == NULL) {
		printf("【ERROR】:File open tfl_log.txt(init_exturn.c)\n");
		return ERROR;
	}
	fclose(fout_tfl_log);

	return SUCCESS;
}
