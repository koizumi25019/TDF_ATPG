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
#include"../Header/Define.h"

//=========================================================================
//  �֐��� : enqueue
//  �@  �\ : �L���[��NLIST������
//  �߂�l : �Ȃ�
//  ��  �� : NLIST�C�^�C�v�i0�F�w�肳�ꂽ�M�����̂݁C1:�o�͐��S���C2�F���͐��S���j
//=========================================================================
void enqueue(NLIST *data, int mode) {

	int i;

	switch (mode) {

	case 0:
		Que.que[Que.rear] = data;
		Que.num += 1;
		Que.rear += 1;
		if (Que.max == Que.rear) {
			Que.rear = 0;
		}
		if (Que.max < Que.num) {
			printf("Que�FOver Flow\n");
			exit(EXIT_FAILURE);
		}
		break;

	case 1:
		for (i = 0; i < data->n_out; i++) {
			Que.que[Que.rear] = data->out[i];
			Que.num += 1;
			Que.rear += 1;
			if (Que.max == Que.rear) {
				Que.rear = 0;
			}
			if (Que.max < Que.num) {
				printf("Que�FOver Flow\n");
				exit(EXIT_FAILURE);
			}
		}
		break;

	case 2:
		for (i = 0; i < data->n_in; i++) {
			Que.que[Que.rear] = data->in[i];
			Que.num += 1;
			Que.rear += 1;
			if (Que.max == Que.rear) {
				Que.rear = 0;
			}
			if (Que.max < Que.num) {
				printf("Que�FOver Flow\n");
				exit(EXIT_FAILURE);
			}
		}
		break;

	}

}

//=========================================================================
//  �֐��� : dequeue
//  �@  �\ : �L���[����1��NLIST�����o��
//  �߂�l : NLIST
//  ��  �� : �Ȃ�
//=========================================================================
NLIST* dequeue() {

	NLIST *p;

	p = *(Que.que + Que.front);

	*(Que.que + Que.front) = NULL;

	Que.num -= 1;
	Que.front += 1;

	if (Que.max == Que.front) {
		Que.front = 0;
	}

	return p;

}


//=========================================================================
//  �֐��� : queue_emp
//  �@  �\ : �X�^�b�N���󂩔���
//  �߂�l : 0�i���j�C1�i�L�j
//  ��  �� : �Ȃ�
//=========================================================================
int queue_emp() {

	if (Que.front == Que.rear) {
		return 0;
	}

	return 1;

}