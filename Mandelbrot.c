/* �������w���KII Mandelbrot.c */
/* �}���f���u�� �W���΍�+�F����]+�摜�}�b�v�ێ� */
/* 2012.01.16, 10RU004, �΋��ˑ� */

#include "Mandelbrot.h"
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

queue *queuepro(unsigned short, unsigned short, color *);
char bmpsave(const char *, unsigned short, unsigned short, color *, unsigned char);

__declspec(dllexport) char Mandelbrot(const char *file, unsigned long mode, double A, double B, double power, double EP,
				double XMIN, double XMAX, double YMIN, double YMAX, unsigned short XSIZE, unsigned short YSIZE)
{
	unsigned char flag, mjmode = mode & 15, colmode = mode >> 4 & 15,
		kill = mode >> 8 & 1, bmp = mode >> 9 & 1, latch = mode >> 12 & 1;
	unsigned short countZ, countT;
	unsigned long count;
	double x, y, xt, yt, xt_, yt_, *zt, sT, fT, inf;
	static double ZMIN = DBL_MAX, ZMAX = -DBL_MAX;
	color *maps, gray = {128, 128, 128}, white = {255, 255, 255};
	queue *now;

	if (EP < 0)	/* Min-Max�̏����� ������s�s�v */
	{
		ZMIN =  DBL_MAX;
		ZMAX = -DBL_MAX;
		return 0;
	}

	if ((zt = malloc(sizeof(double) * TMAX)) == NULL) return 1;	/* Z�̕ۊǌɊm�� */
	if ((maps = malloc(sizeof(color) * XSIZE * YSIZE)) == NULL) return 1;	/* �t���O���F�}�b�v�m�� */

	/* �}�b�v���O���C(���v�Z�_�t���O)�œh��Ԃ� */
	for(count = 0; count < XSIZE * YSIZE; count++) maps[count] = gray;

	now = queuepro(XSIZE, YSIZE, NULL);	/* �L���[�̐U�蕪������(�L�����W�͈�)�o�^ */

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
		x = (XMAX - XMIN) * now->x / XSIZE + XMIN;
		y = (YMAX - YMIN) * now->y / YSIZE + YMIN;

		if (mjmode >> 3 & 1)
		{
				xt_ = x;
				yt_ = y;
		}
		else
		{
				xt_ = yt_ = 0.0;
				A = x;
				B = y;
		}

		flag = true;

		for(countZ = 0; countZ < TMAX; countZ++)
		{
			if ((mjmode & 7) == 1)
			{
				if (xt_ < 0) xt_ = -xt_;
				if (yt_ < 0) yt_ = -yt_;
			}
			xt = xt_ * xt_ - yt_ * yt_ + A;

			if ((mjmode & 7) == 2) yt = -xt_ * yt_ * 2 + B;
			else yt = xt_ * yt_ * 2 + B;

			xt_ = xt;
			yt_ = yt;

			if ((zt[countZ] = sqrt(xt * xt + yt * yt)) > EP)
			{					
				switch(colmode >> 1)
				{
					case 0:	/* �ŏI�ω��� */
						inf = (zt[countZ - 0] - zt[countZ - 1]) / EP;
						break;

					case 1:	/* �W�{���U */
					case 2:	/* �W���΍� */
						countZ++;
						sT = fT = 0.0;
						for(countT = 0; countT < countZ; sT += zt[countT++]);
						sT /= countZ;
						for(countT = 0; countT < countZ; fT += (zt[countT] - sT) * (zt[countT++] - sT));
						inf = fT / countZ;
						break;

					case 3:	/* ���U�ɗv�����񐔂̋t�� */
						inf = -1.0 / (countZ + 1);
						break;
				}

				if (colmode >> 1 == 2) inf = sqrt(inf);	/* �W���΍� */

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
				maps[now->x + XSIZE * now->y].g = (unsigned char)(255 * (cos(inf - M_PI * 2 / 3) + 1) / 2);
				maps[now->x + XSIZE * now->y].b = (unsigned char)(255 * (cos(inf - M_PI * 4 / 3) + 1) / 2);
				/*maps[now->x + XSIZE * now->y].r = (unsigned char)(countZ % 16) * 16;
				maps[now->x + XSIZE * now->y].g = (unsigned char)(countZ % 32) * 8;
				maps[now->x + XSIZE * now->y].b = (unsigned char)(countZ % 64) * 4;*/
			}
			/* ���U���Ȃ������̂ŃL���[�ǉ����� �l��(�Ȉ�) */
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
		now = queuepro(XSIZE, YSIZE, NULL);	/* �L���[��1���炷(�J��) */
	}

	if (kill == false) bmpsave(file, XSIZE, YSIZE, maps, bmp);
	free(maps);
	return 0;
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

char bmpsave(const char *file, unsigned short XSIZE, unsigned short YSIZE, color *maps, unsigned char bmp)
{
	unsigned short BMPHEADERm[4] = {0, 0, 1, 24};
	unsigned long BMPHEADER[10];
	unsigned short countZ, countT;
	unsigned long count;
	FILE *fp;

	/* �t�@�C���������ݏ��� */
	if ((fp = fopen(file, "wb")) == NULL) return 1;

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
				if (maps[count].r == 128 && maps[count].g == 128 && maps[count].b == 128)
					fprintf(fp, "%c%c%c", 255, 255, 255);
				else fwrite(&maps[count], sizeof(color), 1, fp);

			/* 1���C���͕K��4�o�C�g�̔{���� */
			for(count = 0; count < (4 - XSIZE * sizeof(color) % 4) % 4; count++)
				fprintf(fp, "%c", 0);
		}
	}
	else
	{
		/* P3�o�� */
		/*fprintf(fp, "P3\n# 10RU004 �΋��ˑ�\n%d %d\n255\n", XSIZE, YSIZE);

		for(count = 0; count < XSIZE * YSIZE; count++)
			if (maps[count].r == 128 && maps[count].g == 128 && maps[count].b == 128)
				 fprintf(fp, "%03d %03d %03d ", 255, 255, 255);
			else fprintf(fp, "%03d %03d %03d ", maps[count].r, maps[count].g, maps[count].b);*/

		/* P6�o�� */
		fprintf(fp, "P6\n# 10RU004 �΋��ˑ�\n%d %d\n255\n", XSIZE, YSIZE);

		for(count = 0; count < XSIZE * YSIZE; count++)
			/* ���U�őł��~�߂������Ɏc�����O���C�͔����h�� */
			if (maps[count].r == 128 && maps[count].g == 128 && maps[count].b == 128)
				 fprintf(fp, "%c%c%c", 255, 255, 255);
			else fprintf(fp, "%c%c%c", maps[count].r, maps[count].g, maps[count].b);
	}
	fclose(fp);
	return 0;
}