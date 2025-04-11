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
#include"../../Header/Define.h"

//===================================================
// �֐���: fault_select_transition
// �@�\: �J�ڌ̏�̖����o�̏�W�����烉���_���ɖڕW�̏��I�����A�\���̂Ɋi�[
//       unsat_flag �� ON �̏ꍇ�A�P��̏��ڕW�̏�Ƃ��Đݒ�
// �߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
// �����Fint unsat_flag - UNSAT�t���O (ON �܂��� OFF)
//===================================================

int fault_select(int unsat_flag) {
    int i, j;
    int index = 0;
    int select_faults_num;                     // ���ۂɑI�����ꂽ�̏ᐔ
    int target_limit = fi[TARGET_LIMIT].value; // �̏�I������l
    UNDET_FAULT_LIST* current;
    UNDET_FAULT_LIST* tfl_node;
    UNDET_FAULT_LIST* head = NULL;
    UNDET_FAULT_LIST** rand_node;

    // �P��ɐ؂�ւ��ăe�X�g����
    if (unsat_flag == ON) {
        // �����o�̏჊�X�g�̐擪�̌̏��ڕW�̏�Ƃ��Đݒ�
        if (ufl == NULL) {
            printf("�yWARNING�z: No undetected faults available.\n");
            return ERROR;
        }

        // �m�[�h���R�s�[���� tfl ���X�g�ɒǉ�
        tfl_node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));
        if (tfl_node == NULL) {
            printf("�yERROR�z: Memory allocation failed.\n");
            return ERROR;
        }

        // ���̃R�s�[
        tfl_node->type = ufl->type;
        strcpy(tfl_node->name, ufl->name);
        tfl_node->netlist = ufl->netlist;
        tfl_node->node_id = ufl->node_id;
        tfl_node->netlist->tf_flag = FLAG_UP;//�ڕW�̏�I���t���O
        tfl_node->next = NULL;

        tfl = tfl_node;
        tfl_num = 1;

        printf("�yINFO�z: Switched to single fault test generation.\n");
        return SUCCESS;
    }

    // �����o�̏Ⴊ���݂��Ȃ��ꍇ
    if (ufl_num == 0) {
        printf("�yWARNING�z: No undetected faults available.\n");
        return ERROR;
    }

    // �����o�̏��z��Ɋi�[
    rand_node = (UNDET_FAULT_LIST**)malloc(sizeof(UNDET_FAULT_LIST*) * ufl_num);
    if (rand_node == NULL) {
        printf("�yERROR�z: Memory allocation failed.\n");
        return ERROR;
    }

    // �����N���X�g����z��ɕϊ����Atf_flag��������
    current = ufl;
    for (i = 0; i < ufl_num; i++) {
        if (current == NULL) {
            printf("�yERROR�z: Unexpected end of undetected fault list.\n");
            free(rand_node);
            return ERROR;
        }
        rand_node[i] = current;
        rand_node[i]->netlist->tf_flag = FLAG_DOWN; // tf_flag��������
        current = current->next;
    }

    // �����_��������
    srand((unsigned int)time(NULL));

    // �z����V���b�t��
    for (i = ufl_num - 1; i > 0; i--) {
        j = rand() % (i + 1);
        UNDET_FAULT_LIST* temp = rand_node[i];
        rand_node[i] = rand_node[j];
        rand_node[j] = temp;
    }

    // �̏�I������̐ݒ�
    if (target_limit == 0 || target_limit > ufl_num) {
        select_faults_num = ufl_num; // ����Ȃ��܂��͖����o�̏ᐔ�𒴂���ꍇ�͑S�đI��
    }
    else {
        select_faults_num = target_limit;
    }

    // �̏�����ɑI��
    for (index= 0; index < select_faults_num; index++) {
        if (rand_node[index]->netlist->tf_flag == FLAG_DOWN) {
            rand_node[index]->netlist->tf_flag = FLAG_UP; // �I���ς݃t���O�𗧂Ă�

            // �m�[�h���R�s�[���� tfl ���X�g�ɒǉ�
            tfl_node = (UNDET_FAULT_LIST*)malloc(sizeof(UNDET_FAULT_LIST));
            if (tfl_node == NULL) {
                printf("�yERROR�z: Memory allocation failed.\n");
                free(rand_node);
                return ERROR;
            }

            // ���̃R�s�[
            tfl_node->type = rand_node[index]->type;
            strcpy(tfl_node->name, rand_node[index]->name);
            tfl_node->netlist = rand_node[index]->netlist;
            tfl_node->node_id = rand_node[index]->node_id;
            tfl_node->netlist->tf_flag = rand_node[index]->netlist->tf_flag;
            tfl_node->next = head;
            head = tfl_node;

        }
    }

    // tfl���X�g��ݒ�
    tfl = head;
    tfl_num = index;

    // �������̉��
    free(rand_node);

    printf("�yINFO�z: Randomly selected %d faults as target faults.\n", tfl_num);
    return SUCCESS;
}

