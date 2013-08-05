/* �������w���KII H24_01_13.c */
/* �}���f���u�� �o�̓��j���[ */
/* 2012.01.15, 10RU004, �΋��ˑ� */

#include <time.h>
#include "Mandelbrot.h"

long input(char *, long);

void main(void)
{
	char mjmode, colmode, bmpmode, buf[256], file[256] = "\0", type[2][4] = {"ppm", "bmp"};
	unsigned char count;
	long XSIZE, YSIZE;
	double XMIN, XMAX, YMIN, YMAX, A, B, hoge;

	time_t timer;

	printf("�t�@�C���^�C�v\n1:\tPPM\n2:\tBMP\nOther:\t�I��\n>> ");
	bmpmode = input(buf, 256) - 1;
	if (bmpmode < 0 || bmpmode > 1) return;

	printf("\n�`��T�C�Y�����(%s)\n0�ȉ�:\t�I��\n��) 768,768\n>> ", type[bmpmode]);
	input(buf, 256);
	sscanf(buf, "%d,%d%*[^\n]", &XSIZE, &YSIZE);
	if (XSIZE <= 0) return;
	if (YSIZE <= 0) return;

	printf("\n�}�`�I��\n1:\t�}���f���u���W��\n2:\t�W�����A�W��\n3:\t�o�[�j���O�V�b�v�W��\nOther:\t�I��\n>> ");
	mjmode = input(buf, 256) - 1;
	if (mjmode < 0 || mjmode > 3) return;

	if (mjmode & 1)
	{
		printf("\n�W�����A�W��:A,B�����l�����\n��) 0.2,-0.52\n>> ");
		input(buf, 256);
		sscanf(buf, "%lf,%lf%*[^\n]", &A, &B);
	}

	printf("\n�F�ʃ��[�h�I��\n1:\t�ω���\n2:\t�W�{���U\n3:\t�W���΍�\n4:\t���U�ɗv������\nOther:\t�I��\n>> ");
	colmode = input(buf, 256) - 1;
	if (colmode < 0 || colmode > 3) return;

	printf("\n�`��͈͂����(X��)\n��) -2.13,1.07\n>> ");
	input(buf, 256);
	sscanf(buf, "%lf,%lf%*[^\n]", &XMIN, &XMAX);
	if (XMIN > XMAX) return;

	printf("\n�`��͈͂����(Y��)\n��) -1.6,1.6\n>> ");
	input(buf, 256);
	sscanf(buf, "%lf,%lf%*[^\n]", &YMIN, &YMAX);
	if (YMIN > YMAX) return;

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
	printf("\n�`��͈�:\nX %f to %f\nY %f to %f\n���w�肳��܂���\n", XMIN, XMAX, YMIN, YMAX);

	printf("\n�o�̓t�@�C���������(.%s�������)\n>> ", type[bmpmode]);
	input(file, 256);

	for(count = 0; count < strlen(file); buf[count++] = tolower(file[count]));	/* file��buf�ɃR�s�[���啶�����������揬������ */

	buf[count] = '\0';

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
		sprintf(file, "%s.%s", buf, type[bmpmode]);
	}

	printf("\n�o�̓t�@�C���� : \"%s\"\n�o�͒�(1/2)... ���΂炭���҂���������\n", file);

	Mandelbrot(NULL, 0, 0, 0, 0, 0, 0, 0, 0, (unsigned short)XSIZE, (unsigned short)YSIZE);
	/* �����������F
		0�r�b�g�� Min-Max���p���t���O(false�ŏ�����)
		1�r�b�g�� �}�`�I��
		2�r�b�g�� �}�`�I��
		3�r�b�g�� �F�ʃ��[�h
		4�r�b�g�� �F�ʃ��[�h
		5�r�b�g�� �t�@�C�������Ȃ�(Min-Max�̐ݒ�p)
		6�r�b�g�� �t�@�C�����[�h(true:BMP�Afalse:PPM)
		7�r�b�g�� �ڍ׌���(true:�l�������Afalse:�l��)
	*/
	Mandelbrot(file, 1 | mjmode << 1 | colmode << 3 | 1 << 5 | 1 << 7, A, B,
				2.0, XMIN, XMAX, -YMAX, -YMIN, (unsigned short)XSIZE, (unsigned short)YSIZE);
	printf("�o�͒�(2/2)... ���΂炭���҂���������\n", file);
	if (Mandelbrot(file, 1 | mjmode << 1 | colmode << 3 | bmpmode << 6 | 1 << 7, A, B,
					2.0, XMIN, XMAX, -YMAX, -YMIN, (unsigned short)XSIZE, (unsigned short)YSIZE) != 0)
		 printf("�G���[���������܂���\n");
	else printf("�o�͂��I�����܂���\n");

	return;
}

long input(char *s, long len)
{
	fgets(s, len, stdin);
	s[len - 1] = '\0';

	if (strchr(s, '\n') != 0) s[strlen(s) - 1] = '\0';
	else while(getchar() != '\n');

	return atol(s);
}
