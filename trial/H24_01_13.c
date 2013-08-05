/* �������w���KII H24_01_13.c */
/* �}���f���u�� �W���΍�+�F����]+�o�̓��j���[+�摜�}�b�v�ێ� */
/* 2012.01.13, 10RU004, �΋��ˑ� */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795L	/* VC++6.0�ɒ�`����ĂȂ����� */
#endif

#define true 1
#define false 0
#define TMAX 1000

typedef struct{
	/* BMP�t�H�[�}�b�g�� */
	unsigned char b;
	unsigned char g;
	unsigned char r;
} color;

typedef struct queue_tag{
	unsigned short x;
	unsigned short y;
	struct queue_tag *p;
} queue;

long input(char *, long);
char Mandelbrot(const char *, unsigned char, double, double, double, double, double, double, double, unsigned short, unsigned short);
char clrcmp(color, color);
queue *queuepro(unsigned short, unsigned short, color *);

void main(void)
{
	char mjmode, colmode, bmpmode, buf[256], file[256] = "\0", type[2][4] = {"ppm", "bmp"};
	unsigned char count;
	long XSIZE, YSIZE;
	double XMIN, XMAX, YMIN, YMAX, A = 0.2, B = -0.52, hoge;

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

	Mandelbrot(NULL, false, 0, 0, 0, 0, 0, 0, 0, (unsigned short)XSIZE, (unsigned short)YSIZE);
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
	Mandelbrot(file, true | mjmode << 1 | colmode << 3 | 1 << 5, 0.2,
				-0.52, 2.0, XMIN, XMAX, -YMAX, -YMIN, (unsigned short)XSIZE, (unsigned short)YSIZE);
	printf("�o�͒�(2/2)... ���΂炭���҂���������\n", file);
	if (Mandelbrot(file, true | mjmode << 1 | colmode << 3 | bmpmode << 6 | 1 << 7, 0.2,
					-0.52, 2.0, XMIN, XMAX, -YMAX, -YMIN, (unsigned short)XSIZE, (unsigned short)YSIZE) != 0)
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

char clrcmp(color a, color b)
{
	if (a.r == b.r && a.g == b.g && a.b == b.b) return true;
	return false;
}

queue *queuepro(unsigned short x, unsigned short y, color *maps)
{
	/* �L���[�|�C���^�Ǘ� ���͂�g���񂹂Ȃ��قǂɓ��� */
	static queue *head = NULL, *tail = NULL;
	static unsigned short XSIZE = 0, YSIZE = 0;
	queue *q = head;

	if (maps != NULL)
	{
		/* �L���[��o�^*/
		if (x < 0 || y < 0 || x > XSIZE - 1 || y > YSIZE - 1
			|| XSIZE == 0 || YSIZE == 0) return head;	/* �}�b�v�T�C�Y�O�A�}�b�v�T�C�Y�ݒ肪�Ȃ��ꍇ */

		if (maps[x + XSIZE * y].r != 128
			|| maps[x + XSIZE * y].g != 128
			|| maps[x + XSIZE * y].b != 128)
			return head;	/* �O���C(���v�Z�_)�łȂ����� */

		while(q != NULL)
		{
			if (x == q->x && y == q->y) return head;	/* �L���[�ɓo�^�ς�(���W������) */
			q = q->p;
		}
		if ((q = malloc(sizeof(queue))) == NULL) exit(1);	/* �����܂Ŗʓ|����܂��� */
		if (tail != NULL) tail->p = q;
		else head = q;
		tail = q;
		q->x = x;
		q->y = y;
		q->p = NULL;
	}
	else
	{
		if (x == XSIZE && y == YSIZE && head != NULL)
		{
			/* �L���[���폜*/
			q = head->p;
			if (q == NULL) tail = NULL;	/* ���̃L���[�̌�낪�Ȃ� */
			free(head);
			head = q;
		}
		XSIZE = x;
		YSIZE = y;
	}
	return head;
}

char Mandelbrot(const char *file, unsigned char mode, double A, double B, double EP,
				double XMIN, double XMAX, double YMIN, double YMAX, unsigned short XSIZE, unsigned short YSIZE)
{
	unsigned char flag, mjmode = mode >> 1 & 3, colmode = mode >> 3 & 3,
		kill = mode >> 5 & 1, bmp = mode >> 6 & 1, latch = mode >> 7 & 1;
	unsigned short countZ, countT, BMPHEADERm[4] = {0, 0, 1, 24};
	unsigned long count, BMPHEADER[10];
	double x, y, xt, yt, xt_, yt_, zt[TMAX], sT, fT, inf;
	static double ZMIN = DBL_MAX, ZMAX = -DBL_MAX;
	color *maps, gray = {128, 128, 128}, white = {255, 255, 255};
	queue *now;
	FILE *fp;

	if ((mode & 1) == false)	/* Min-Max�̏����� ������s�s�v */
	{
		ZMIN =  DBL_MAX;
		ZMAX = -DBL_MAX;
		return 0;
	}

	if ((maps = malloc(sizeof(color) * XSIZE * YSIZE)) == NULL) return 1;	/* �t���O���F�}�b�v�m�� */

	/* �}�b�v���O���C(���v�Z�_�t���O)�œh��Ԃ� */
	for(count = 0; count < XSIZE * YSIZE; count++) maps[count] = gray;

	now = queuepro(XSIZE, YSIZE, NULL);	/* �L���[�̐U�蕪�������o�^ */

	for(count = 0; count < XSIZE; count++)	/* �����L���[��o�^ �㉺�ӂ̓_�S�� */
	{
		queuepro(count, 0		 , maps);
		queuepro(count, YSIZE - 1, maps);
	}

	for(count = 0; count < YSIZE; count++)	/* �����L���[��o�^ ���E�ӂ̓_�S�� */
	{
		queuepro(0		  , count, maps);
		queuepro(XSIZE - 1, count, maps);
	}

	now = queuepro(0, 0, maps);	/* �L���[�̃w�b�_�̃|�C���^�擾 */

	while(now != NULL)	/* �L���[���Ȃ��Ȃ�܂ŌJ��Ԃ� */
	{
		if (clrcmp(maps[now->x + XSIZE * now->y], gray))
		{
			x = (XMAX - XMIN) * now->x / XSIZE + XMIN;
			y = (YMAX - YMIN) * now->y / YSIZE + YMIN;

			switch(mjmode & 1)
			{
				case 1:
					xt_ = x;
					yt_ = y;
					break;

				default:
					xt_ = yt_ = 0.0;
					A = x;
					B = y;
					break;
			}

			flag = true;

			for(countZ = 0; countZ < TMAX; countZ++)
			{
				if (mjmode >> 1 & 1)
				{
					if (xt_ < 0) xt_ = -xt_;
					if (yt_ < 0) yt_ = -yt_;
					xt = xt_ * xt_ - yt_ * yt_ + A;
					yt = xt_ * yt_ * 2 + B;
				}
				else
				{
					xt = xt_ * xt_ - yt_ * yt_ + A;
					yt = xt_ * yt_ * 2 + B;
				}
				xt_ = xt;
				yt_ = yt;

				if (zt[countZ] = sqrt(xt * xt + yt * yt) > EP)
				{
					switch(colmode)
					{
						case 0:
							/* �ŏI�ω��� */
							inf = (zt[countZ - 0] - zt[countZ - 1]) / (countZ + 1);
							break;

						case 1:
							/* �W�{���U */
							countZ++;
							sT = fT = 0.0;
							for(countT = 0; countT < countZ; sT += zt[countT++]);
							sT /= countZ;
							for(countT = 0; countT < countZ; fT += (zt[countT] - sT) * (zt[countT++] - sT));
							inf = fT / countZ;
							break;

						case 2:
							/* �W���΍� */
							countZ++;
							sT = fT = 0.0;
							for(countT = 0; countT < countZ; sT += zt[countT++]);
							sT /= countZ;
							for(countT = 0; countT < countZ; fT += (zt[countT] - sT) * (zt[countT++] - sT));
							inf = sqrt(fT / countZ);
							break;

						case 3:
							/* ���U�ɗv������ */
							inf = countZ;
							break;
					}

					if (kill)
					{
						/* Min-Max�̐ݒ� */
						if (ZMIN > inf) ZMIN = inf;
						if (ZMAX < inf) ZMAX = inf;
					}

					flag = false;
					break;
				}
			}

			if (flag) maps[now->x + XSIZE * now->y] = white;	/* ���U�����̂Ŕ��h�� */
			else
			{
				/* Min-Max�̐ݒ莞�͍Œ������ł���F(�O���[�X�P�[���ȊO)�ɂ��� ���Ԃ�������������Z�͔����� */
				if (kill) maps[now->x + XSIZE * now->y].g = 255;
				else
				{
					/* �F�Â� */
					inf = (M_PI * 330 / 180 * (inf - ZMIN) / (ZMAX - ZMIN));
					maps[now->x + XSIZE * now->y].r = (unsigned char)(255 * (cos(inf			   ) + 1) / 2);
					maps[now->x + XSIZE * now->y].g = (unsigned char)(255 * (cos(inf + M_PI * 2 / 3) + 1) / 2);
					maps[now->x + XSIZE * now->y].b = (unsigned char)(255 * (cos(inf + M_PI * 4 / 3) + 1) / 2);
				}
				/* ���U���Ȃ������̂ŃL���[�ǉ����� �O���C(���v�Z�_�t���O)�Ȃ�ǉ� �l��(�Ȉ�) */
				queuepro(now->x + 1, now->y, maps);
				queuepro(now->x - 1, now->y, maps);
				queuepro(now->x, now->y + 1, maps);
				queuepro(now->x, now->y - 1, maps);
				if (latch)	/*  ����(�ڍ�) */
				{
					queuepro(now->x + 1, now->y + 1, maps);
					queuepro(now->x - 1, now->y + 1, maps);
					queuepro(now->x + 1, now->y - 1, maps);
					queuepro(now->x - 1, now->y - 1, maps);
				}
			}
		}
		now = queuepro(XSIZE, YSIZE, false);	/* �L���[��1���炷(�J��) */
	}

	if (kill == false)
	{
		/* �t�@�C���������ݏ��� */
		if ((fp = fopen(file, "wb")) == NULL) return 2;

		if (bmp)
		{
			/* BITMAP�o�� �\���̂̃A���C�����g�����(�N���X�v���b�g�t�H�[�����ƃR���p�C���̎w��ύX�Ƃ�)
			   �ʓ|�Ȃ̂Ŕ�\���̂ŏ����o�� */

			/* BMPFILEHEADER */
			fprintf(fp, "BM");	/* bfType */
			BMPHEADER[0] = 54 + sizeof(color) * XSIZE * YSIZE;	/* bfSize */
			memcpy(&BMPHEADER[1], BMPHEADERm, 4);	/* bfReserved1 + bfReserved2 */
			BMPHEADER[2] = 54;	/* bfOffBits */
			fwrite(BMPHEADER, sizeof(unsigned long), 3, fp);

			/* BMPINFOHEADER */
			BMPHEADER[0] = 40;	/* biSize */
			BMPHEADER[1] = XSIZE;	/* biWidth */
			BMPHEADER[2] = YSIZE;	/* biHeight */
			memcpy(&BMPHEADER[3], &BMPHEADERm[2], 4);	/* biPlanes + biBitCount */
			BMPHEADER[4] = 0;	/* biCompression */
			BMPHEADER[5] = sizeof(color) * XSIZE * YSIZE;	/* biSizeImage */
			BMPHEADER[6] = 0;	/* biXPelsPerMeter */
			BMPHEADER[7] = 0;	/* biYPelsPerMeter */
			BMPHEADER[8] = 0;	/* biClrUsed */
			BMPHEADER[9] = 0;	/* biClrImportant */
			fwrite(BMPHEADER, sizeof(unsigned long), 10, fp);

			/* DATA */
			for(countZ = YSIZE; countZ > 0; countZ--)	/* ��������E��� */
			{
				for(count = countZ * XSIZE - XSIZE; count < XSIZE + countZ * XSIZE - XSIZE; count++)
					/* ���U�őł��~�߂������Ɏc�����O���C�͔����h�� */
					if (clrcmp(maps[count], gray))
						fprintf(fp, "%c%c%c", 255, 255, 255);
					else fwrite(&maps[count], sizeof(color), 1, fp);

				for(count = 0; count < XSIZE * 3 % 4; count++)
					fprintf(fp, "%c", 255);	/* 1���C���͕K��4�o�C�g�̔{���� */
			}
		}
		else
		{
			/* P3�o�� */
			fprintf(fp, "P3\n# 10RU004 �΋��ˑ�\n%d %d\n255\n", XSIZE, YSIZE);

			for(count = 0; count < XSIZE * YSIZE; count++)
				/* ���U�őł��~�߂������Ɏc�����O���C�͔����h�� */
				if (clrcmp(maps[count], gray)) fprintf(fp, "%03d %03d %03d ", 255, 255, 255);
				else fprintf(fp, "%03d %03d %03d ", maps[count].r, maps[count].g, maps[count].b);

			/* P6�o�� �Ȃ��������(PPM�o�C�i���t�H�[�}�b�g�̗����ł��ĂȂ�? IrfanView���̖��?)�̂ŕ��u�i���� */
			/*fprintf(fp, "P6\n# 10RU004 �΋��ˑ�\n%d %d\n255\n", XSIZE, YSIZE);

			for(count = 0; count < XSIZE * YSIZE; count++)
				if (clrcmp(maps[count], gray)) fprintf(fp, "%c%c%c", 255, 255, 255);
				else fprintf(fp, "%c%c%c", maps[count].r, maps[count].g, maps[count].b);
			*/
		}
		fclose(fp);
	}
	free(maps);

	return 0;
}
