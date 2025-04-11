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
//�֐���:standard_output
//�@�\:�W���o��
//�߂�l�F�Ȃ�
//�����F�Ȃ�
//===================================================
void standard_output(void) {

	printf("//---------------------------------------------------\n");
	printf("//	   Transition Fault ATPG   Information\n");
	printf("//---------------------------------------------------\n");
	printf("// Target Circuit              :%s\n", fi[CIRCUIT].name);
	printf("// Fault Type                  :NODE\n");
	if (fi[TARGET_LIMIT].value == 1) {
		printf("// Test Generation             :STTG\n");
	}
	else {
		printf("// Test Generation             :MTTG\n");
	}
	printf("// Number of Total Faults      :%d\n", (int)all_num);
	printf("// Number of Untestable Faults :%d\n", (int)red_num);
	printf("// Number of High Power Faults :%d\n", (int)hpfl_num);
	printf("// Number of Detected Faults   :%d\n", (int)dfl_num);
	printf("// Number of Undetected Faults :%d\n", (int)ufl_num);
	printf("// Number of Test Vectors      :%d\n", tp_num);
	printf("// Rate of Detected Faults     :%.2lf%%\n",(((double)dfl_num)/ ((double)all_num))*100.0);
	printf("// Elapsed time                :%.2lfsec\n", (double)clock() / CLOCKS_PER_SEC);
	printf("// Time to abort solver        :%dsec\n", fi[TIME].value);
	printf("// Target faults limit         :%d\n", fi[TARGET_LIMIT].value);
	printf("// Rand Optiont                :%s\n", fi[RAND].name);
	printf("// Low Power Mode              :%s\n", fi[LOW_POWER].name);
	printf("// WSA                         :%d[��]\n", fi[WSATH].value);
	printf("//---------------------------------------------------\n");
}