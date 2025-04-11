//------------------------------------------------------------------------
//�C���N���[�h
//------------------------------------------------------------------------
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//------------------------------------------------------------------------
//����C���N���[�h
//------------------------------------------------------------------------
#include"../Header/Define.h"


//===================================================
//�֐���:cmdline
//�@�\:�R�}���h���C�����
//�߂�l�Fint ����I��:SUCCESS(=0) , �ُ�I��:ERROR(=-1)
//�����F�R�}���h���C�������̐�, �R�}���h���C������
//===================================================
int cmdline(int argc, char *argv[])
{
	int i;

	//------------------------------------------------------------------------
	// ���s�R�}���h���
	//------------------------------------------------------------------------
	for (i = 1; i < argc; i++)
	{

		//------------------------------------------------------------------------
		// ���̓t�@�C��
		//------------------------------------------------------------------------

		/* ��H�t�@�C�����̓o�^ */
		if (!strcmp(argv[i], "-net")) {
			i++;
			fi[CIRCUIT].name = argv[i];
			fi[CIRCUIT].flag = ON;
		}

		//------------------------------------------------------------------------
		// �I�v�V����
		//------------------------------------------------------------------------

		/* �����d�̓��[�h�擾 */
		else if (!strcmp(argv[i], "-power")) {
			i++;
			fi[LOW_POWER].name = argv[i];
			fi[LOW_POWER].flag = ON;
		}

		/* �t�H���_�[�̎w�� */
		else if (!strcmp(argv[i], "-folder")) {
			i++;
			fi[FOLDER].name = argv[i];
			fi[FOLDER].flag = ON;
		}

		/* �e�X�g�s�\�̏�폜���[�h(default:ON)�擾 */
		else if (!strcmp(argv[i], "-untest_mode")) {
			i++;
			fi[UNTEST_MODE].name = argv[i];
			fi[UNTEST_MODE].flag = ON;
		}

		/* �����_�����d�̏�I��(default:OFF)�擾 */
		else if (!strcmp(argv[i], "-rand")) {
			i++;
			fi[RAND].name = argv[i];
			fi[RAND].flag = ON;
		}

		/* Solver�ł��؂莞��(default:10)�擾 */
		else if (!strcmp(argv[i], "-time")) {
			i++;
			fi[TIME].value = atoi(argv[i]);
			fi[TIME].flag = ON;
		}

		/* WSA臒l������d��臒l(default:20)�擾 */
		else if (!strcmp(argv[i], "-wsath")) {
			i++;
			fi[WSATH].value = atoi(argv[i]);
			fi[WSATH].flag = ON;
		}

		/* �ڕW�̏�̏���擾(0�̏ꍇ�͐����Ȃ�) */
		else if (!strcmp(argv[i], "-target_limit")) {
			i++;
			fi[TARGET_LIMIT].value = atoi(argv[i]);
			fi[TARGET_LIMIT].flag = ON;
		}

		/*�̏�t�@�C���擾*/
		else if (!strcmp(argv[i], "-fault")) {
			i++;
			fi[FAULT].name = argv[i];
			fi[FAULT].flag = ON;
		}


		/* ����`�̃I�v�V�������w�肳�ꂽ�ꍇ */
		else
		{
			printf("Option�y%s�zis not defined! ����`�̃I�v�V��������`����܂����I\n", argv[i]);
			return ERROR;
		}

	}


	//------------------------------------------------------------------------
	// �K�v�R�}���h�̓��͊m�F
	//------------------------------------------------------------------------
	if (fi[CIRCUIT].flag == OFF) { printf("��H�t�@�C��������͂��Ă�������\n");		return ERROR; }
	if (fi[LOW_POWER].flag == OFF) { printf("�����d�͎w�����ۂ�����͂��Ă�������\n");		return ERROR; }
	if (fi[FOLDER].flag == OFF) { printf("�o�̓t�H���_�[������͂��Ă�������\n");		return ERROR; }
	//------------------------------------------------------------------------
	// default�̐ݒ�
	//------------------------------------------------------------------------
	if (fi[UNTEST_MODE].flag == OFF) {
		fi[UNTEST_MODE].name = "ON";
	}
	if (fi[RAND].flag == OFF) {
		fi[RAND].name = "OFF";
	}
	if (fi[TIME].flag == OFF) {
		fi[TIME].value = 10;
	}
	if (fi[WSATH].flag == OFF) {
		fi[WSATH].value = 20;
	}

	return SUCCESS;

}