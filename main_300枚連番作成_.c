/* �������w���KII H24_01_13.c */
/* �}���f���u�� �o�̓��j���[ */
/* 2012.01.17, 10RU004, �΋��ˑ� */

#include <time.h>
#include "Mandelbrot.h"

void main(void)
{
	char mjmode, colmode, bmpmode, buf[256], file[256] = "\0", type[2][4] = {"ppm", "bmp"};
	unsigned short count, couMAX = 300;
	unsigned short XSIZE, YSIZE;
	double XMIN = -1.5, XMAX = 1.5, YMIN = -1.5, YMAX = 1.5, A = 0.2, B = -0.52, hoge,
		XMIN_ = -1.2, XMAX_ = 1.2, YMIN_ = -1.2, YMAX_ = 1.2;

	time_t timer;

	//printf("�t�@�C���^�C�v\n1:\tPPM\n2:\tBMP\nOther:\t�I��\n>> ");
	bmpmode = 1;

	YSIZE = XSIZE = 500;

	//printf("\n�}�`�I��\n1:\t�}���f���u���W��\n2:\t�o�[�j���O�V�b�v�W��\n3:\t�g���C�R�[���W��\nOther:\t�I��\n>> ");
	mjmode = 2 | 1 << 3;

	//printf("\n�F�ʃ��[�h�I��\n1:\t�ω���\n2:\t�W�{���U\n3:\t�W���΍�\n4:\t���U�ɗv�����񐔂̋t��\nOther:\t�I��\n>> ");
	colmode = 3;

	if (YSIZE * (XMAX - XMIN) < XSIZE * (YMAX - YMIN))
	{
		hoge = (XSIZE * (YMAX - YMIN) / YSIZE - XMAX + XMIN) / 2;
		XMAX += hoge;
		XMIN -= hoge;
	}
	else
	{
		hoge = (YSIZE * (XMAX - XMIN) / XSIZE - YMAX + YMIN) / 2;
		YMAX += hoge;
		YMIN -= hoge;
	}
	printf("�`��͈�:\nX %f to %f\nY %f to %f\n���w�肳��܂���\n", XMIN, XMAX, YMIN, YMAX);

	buf[count = 0] = '\0';

	/*if (count < 5 || buf[count - 4] != '.' || strcmp(&buf[count - 3], type[bmpmode]) != 0)*/
	if (!(count > 4 && buf[count - 4] == '.' && strcmp(&buf[count - 3], type[bmpmode]) == 0))
	{
		if (count == 0)
		{
			time(&timer);
			sprintf(buf, "%s", ctime(&timer));
			for(count = 0; count < strlen(buf); count++)
			{
				if (buf[count] == ':') buf[count] = '_';	/* ctime�֐��ŕԂ������񒆂̃t�@�C�����Ɏw��ł��Ȃ�������u�� */
				if (buf[count] == '\n') buf[count] = '\0';
			}
		}
		sprintf(file, "%s", buf);
	}

	printf("\n�o�̓t�@�C���� : \"%s\"\n�o�͒�(1/2)... ���΂炭���҂���������\n", file);

	Mandelbrot(NULL, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0);
	/* �����������F
		��	��
		00	1�r�b�g�� �}�`�I��
		01	2�r�b�g�� �}�`�I��
		02	X�r�b�g�� �}�`�I��(�\��)
		03	9�r�b�g�� �}�`�I��(true:�W�����A�Afalse:normal)
		04	8�r�b�g�� �F���[�h(true:color�Afalse:gray)
		05	3�r�b�g�� �F�ʃ��[�h
		06	4�r�b�g�� �F�ʃ��[�h
		07	X�r�b�g�� �F�ʃ��[�h(�\��)
		08	5�r�b�g�� �t�@�C�������Ȃ�(Min-Max�̐ݒ�p)
		09	6�r�b�g�� �t�@�C�����[�h(true:BMP�Afalse:PPM)
		10	X�r�b�g�� (�\��)
		11	X�r�b�g�� (�\��)
		12	7�r�b�g�� �ڍ׌���(true:�l�������Afalse:�l��)
		13	X�r�b�g�� (�\��)
		14	X�r�b�g�� (�\��)
		15	X�r�b�g�� (�\��)
			0�r�b�g�� Min-Max���p���t���O(false�ŏ�����)
	*/

	for(count = 0; count < couMAX; count++)
	{
		printf("PLOT %d\n", count+1);
		Mandelbrot(file, mjmode | colmode << 5 | 1 << 8 | 1 << 12,
			(XMAX_ - XMIN_) * count / (couMAX - 1) + XMIN_,
			(YMAX_ - YMIN_) * count / (couMAX - 1) + YMIN_,
			2.0, 2.0, XMIN, XMAX, YMIN, YMAX, XSIZE, YSIZE);
	}

	printf("�o�͒�(2/2)... ���΂炭���҂���������\n", file);

	for(count = 0; count < couMAX; count++)
	{
		printf("PLOT %d\n", count+1);
		sprintf(buf, "%s (%03d).bmp", file, count+1);
		Mandelbrot(buf, mjmode | colmode << 5 | bmpmode << 9 | 1 << 12,
			(XMAX_ - XMIN_) * count / (couMAX - 1) + XMIN_,
			(YMAX_ - YMIN_) * count / (couMAX - 1) + YMIN_,
			2.0, 2.0, XMIN, XMAX, YMIN, YMAX, XSIZE, YSIZE);
	}

	return;
}
