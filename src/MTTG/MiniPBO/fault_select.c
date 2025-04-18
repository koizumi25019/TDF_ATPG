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

//===================================================
// 関数名: fault_select_transition
// 機能: 遷移故障の未検出故障集合からランダムに目標故障を選択し、構造体に格納
//       unsat_flag が ON の場合、単一故障を目標故障として設定
// 戻り値：int 正常終了:SUCCESS(=0) , 異常終了:ERROR(=-1)
// 引数：int unsat_flag - UNSATフラグ (ON または OFF)
//===================================================

int fault_select(int unsat_flag) {
    int i, j;
    int index = 0;
    int select_faults_num;                     // 実際に選択された故障数
    int target_limit = fi[TARGET_LIMIT].value; // 故障選択上限値
    UNDET_FAULT_LIST* current;
    UNDET_FAULT_LIST* tfl_node;
    UNDET_FAULT_LIST* head = NULL;
    UNDET_FAULT_LIST** rand_node;

    // 単一に切り替えてテスト生成
    if (unsat_flag == ON) {
        // 未検出故障リストの先頭の故障を目標故障として設定
        if (ufl == NULL) {
            printf("【WARNING】: No undetected faults available.\n");
            return ERROR;
        }

        // ノードをコピーして tfl リストに追加
        tfl_node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));
        if (tfl_node == NULL) {
            printf("【ERROR】: Memory allocation failed.\n");
            return ERROR;
        }

        // 情報のコピー
        tfl_node->type = ufl->type;
        strcpy(tfl_node->name, ufl->name);
        tfl_node->netlist = ufl->netlist;
        tfl_node->node_id = ufl->node_id;
        tfl_node->netlist->tf_flag = FLAG_UP;//目標故障選択フラグ
        tfl_node->next = NULL;

        tfl = tfl_node;
        tfl_num = 1;

        printf("【INFO】: Switched to single fault test generation.\n");
        return SUCCESS;
    }

    // 未検出故障が存在しない場合
    if (ufl_num == 0) {
        printf("【WARNING】: No undetected faults available.\n");
        return ERROR;
    }

    // 未検出故障を配列に格納
    rand_node = (UNDET_FAULT_LIST**)malloc(sizeof(UNDET_FAULT_LIST*) * ufl_num);
    if (rand_node == NULL) {
        printf("【ERROR】: Memory allocation failed.\n");
        return ERROR;
    }

    // リンクリストから配列に変換し、tf_flagを初期化
    current = ufl;
    for (i = 0; i < ufl_num; i++) {
        if (current == NULL) {
            printf("【ERROR】: Unexpected end of undetected fault list.\n");
            free(rand_node);
            return ERROR;
        }
        rand_node[i] = current;
        rand_node[i]->netlist->tf_flag = FLAG_DOWN; // tf_flagを初期化
        current = current->next;
    }

    // ランダム初期化
    srand((unsigned int)time(NULL));

    // 配列をシャッフル
    for (i = ufl_num - 1; i > 0; i--) {
        j = rand() % (i + 1);
        UNDET_FAULT_LIST* temp = rand_node[i];
        rand_node[i] = rand_node[j];
        rand_node[j] = temp;
    }

    // 故障選択上限の設定
    if (target_limit == 0 || target_limit > ufl_num) {
        select_faults_num = ufl_num; // 上限なしまたは未検出故障数を超える場合は全て選択
    }
    else {
        select_faults_num = target_limit;
    }

    // 故障を順に選択
    for (index= 0; index < select_faults_num; index++) {
        if (rand_node[index]->netlist->tf_flag == FLAG_DOWN) {
            rand_node[index]->netlist->tf_flag = FLAG_UP; // 選択済みフラグを立てる

            // ノードをコピーして tfl リストに追加
            tfl_node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));
            if (tfl_node == NULL) {
                printf("【ERROR】: Memory allocation failed.\n");
                free(rand_node);
                return ERROR;
            }

            // 情報のコピー
            tfl_node->type = rand_node[index]->type;
            strcpy(tfl_node->name, rand_node[index]->name);
            tfl_node->netlist = rand_node[index]->netlist;
            tfl_node->node_id = rand_node[index]->node_id;
            tfl_node->netlist->tf_flag = rand_node[index]->netlist->tf_flag;
            tfl_node->next = head;
            head = tfl_node;

        }
    }

    // tflリストを設定
    tfl = head;
    tfl_num = index;

    // メモリの解放
    free(rand_node);

    printf("【INFO】: Randomly selected %d faults as target faults.\n", tfl_num);
    return SUCCESS;
}

