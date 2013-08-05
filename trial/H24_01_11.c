/* �������w���KII H24_01_11.c */
/* �}���f���u�� �W���΍�+�F����]+�o�̓��j���[+�摜�}�b�v�ێ� */
/* 2012.01.11, 10RU004, �΋��ˑ� */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define true 1
#define false 0
#define TMAX 1000
#define XSIZE 768
#define YSIZE 768

typedef struct{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} color;

typedef struct queue_tag{
	long long x;
	long long y;
	struct queue_tag *p;
} queue;

long input(char *, long);
char Mandelbrot(char *, unsigned char, double, double, double, double, double, double, double);
char clrcmp(color, color);
queue *queuepro(long long, long long, char);

void main(void)
{
	char mjmode, colmode, file[256] = "\0", buf[256];
	unsigned char count;
	double XMIN, XMAX, YMIN, YMAX, A = 0.2, B = -0.52, hoge;

	time_t timer;

	printf("�}�`�I��\n1:\t�}���f���u���W��\n2:\t�W�����A�W��\nOther:\t�I��\n>> ");
	mjmode = input(buf, 256) - 1;
	if (mjmode < 0 || mjmode > 1) return;

	printf("\n�F�ʃ��[�h�I��\n1:\t�ω���\n2:\t�W���΍�\nOther:\t�I��\n>> ");
	colmode = input(buf, 256) - 1;
	if (colmode < 0 || colmode > 1) return;

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

	printf("\n�o�̓t�@�C���������(.ppm�������)\n>> ");
	input(file, 256);

	for(count = 0; count < strlen(file); buf[count++] = tolower(file[count]));	/* file��buf�ɃR�s�[���啶�����������揬������ */

	buf[count] = '\0';

	if (count < 5 || strcmp(&buf[count - 4], ".ppm") != 0)
	{
		if (count == 0)
		{
			time(&timer);
			sprintf(buf, "%s", ctime(&timer));
			for(count = 0; count < strlen(buf); count++)
				if (buf[count] == ':' || buf[count] == '\n') buf[count] = '_';	/* ctime�֐��ŕԂ������񒆂̃t�@�C�����Ɏw��ł��Ȃ�������u�� */
		}
		sprintf(file, "%s.ppm", buf);
	}

	printf("\n�o�̓t�@�C���� : %s\n�o�͒�... ���΂炭���҂���������\n", file);

	Mandelbrot(NULL, false, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	/* �����������F
		1�r�b�g�� Min-Max���p���t���O(false�ŏ�����)
		2�r�b�g�� �}�`�I��
		3�r�b�g�� �F�ʃ��[�h
		4�r�b�g�� �t�@�C�������Ȃ�(Min-Max�̐ݒ�p)
	*/
	Mandelbrot(file, true | mjmode << 1 | colmode << 2 | 1 << 3, 0.2, -0.52, 2.0, XMIN, XMAX, YMIN, YMAX);
	if (Mandelbrot(file, true | mjmode << 1 | colmode << 2, 0.2, -0.52, 2.0, XMIN, XMAX, YMIN, YMAX) == false)
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

queue *queuepro(long long x, long long y, char mode)
{
	/* �L���[�|�C���^�Ǘ� */
	static queue *head = NULL, *tail = NULL;
	queue *q = head;

	switch(mode)
	{
	case true:
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
		break;

	case false:
		q = head->p;
		free(head);
		head = q;
		if (head == NULL) tail = NULL;
		break;

	default:
		break;
	}
	return head;
}

char Mandelbrot(char *file, unsigned char mode, double A, double B, double EP, double XMIN, double XMAX, double YMIN, double YMAX)
{
	char flag, mjmode = ((mode & 2) != 0), colmode = ((mode & 4) != 0), kill = ((mode & 8) != 0);
	long long countZ, countT, count = 1;
	double x, y, xt, yt, xt_, yt_, zt[TMAX], sT, fT, inf;
	static double ZMIN = DBL_MAX, ZMAX = -DBL_MAX;
	color *maps, gray = {128, 128, 128};
	queue *now;
	FILE *fp;

	if ((mode & 1) == false)	/* Min-Max�̏����� ������s�s�v */
	{
		ZMIN =  DBL_MAX;
		ZMAX = -DBL_MAX;
		return true;
	}

	if ((maps = malloc(sizeof(color) * XSIZE * YSIZE)) == NULL) return false;	/* �t���O���F�}�b�v�m�� */

	for(countT = 0; countT < XSIZE * YSIZE; countT++) maps[countT] = gray;	/* �}�b�v���O���C(���v�Z�_�t���O)�œh��Ԃ� */

	for(countT = 0; countT < XSIZE; countT++)	/* �����L���[��o�^ �㉺�ӂ̓_�S�� */
	{
		queuepro(countT, 0		  , true);
		queuepro(countT, YSIZE - 1, true);
	}

	for(countT = 0; countT < YSIZE; countT++)	/* �����L���[��o�^ ���E�ӂ̓_�S�� */
	{
		queuepro(0		  , countT, true);
		queuepro(XSIZE - 1, countT, true);
	}

	now = queuepro(0, 0, true);	/* �L���[�̃w�b�_�̃|�C���^�擾 */

	do {	/* �L���[���Ȃ��Ȃ�܂ŌJ��Ԃ� */
		x = (XMAX - XMIN) * now->x / XSIZE + XMIN;
		y = (YMAX - YMIN) * now->y / YSIZE + YMIN;

		switch(mjmode)
		{
			case false:
				xt_ = yt_ = 0.0;
				A = x;
				B = y;
				break;

			case true:
				xt_ = x;
				yt_ = y;
				break;

			default:
				break;
		}

		flag = true;

		for(countZ = 0; countZ < TMAX; countZ++)
		{
			xt = xt_ * xt_ - yt_ * yt_ + A;
			yt = xt_ * yt_ * 2 + B;
			xt_ = xt;
			yt_ = yt;

			if (zt[countZ] = sqrt(xt * xt + yt * yt) > EP)
			{
				switch(colmode)
				{
					case false:
						/* �ŏI�ω��� */
						inf = zt[countZ] - zt[countZ - 1];
						break;

					case true:
						/* �W���΍� */
						countZ++;
						sT = fT = 0.0;
						for(countT = 0; countT < countZ; sT += zt[countT++]);
						sT /= countZ;
						for(countT = 0; countT < countZ; fT += (zt[countT] - sT) * (zt[countT++] - sT));
						inf = sqrt(fT / countZ);
						break;

					default:
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

		if (flag)
			/* ���U�����̂Ŕ��h�� */
			  maps[now->x + XSIZE * now->y].r
			= maps[now->x + XSIZE * now->y].g
			= maps[now->x + XSIZE * now->y].b
			= 255;
		else
		{
			/* Min-Max�̐ݒ莞�͍Œ������ł���F(�O���C�ȊO)�ɂ��� ���Ԃ�������������Z�͔����� */
			if (kill) maps[now->x + XSIZE * now->y].g = 255;
			else
			{
				/* �F�Â� */
				inf = (M_PI * 330 / 180 * (inf - ZMIN) / (ZMAX - ZMIN));
				maps[now->x + XSIZE * now->y].r = (unsigned char)(255 * (cos(inf			   ) + 1) / 2);
				maps[now->x + XSIZE * now->y].g = (unsigned char)(255 * (cos(inf + M_PI * 2 / 3) + 1) / 2);
				maps[now->x + XSIZE * now->y].b = (unsigned char)(255 * (cos(inf + M_PI * 4 / 3) + 1) / 2);
			}
			/* ���U���Ȃ������̂ŃL���[�ǉ����� �O���C(���v�Z�_�t���O)�Ȃ�ǉ� */
			if (now->x < XSIZE)	if (clrcmp(maps[(now->x + 1) + XSIZE * now->y], gray)) queuepro(now->x + 1, now->y, true);
			if (now->x > 0)		if (clrcmp(maps[(now->x - 1) + XSIZE * now->y], gray)) queuepro(now->x - 1, now->y, true);
			if (now->y < YSIZE)	if (clrcmp(maps[now->x + XSIZE * (now->y + 1)], gray)) queuepro(now->x, now->y + 1, true);
			if (now->y > 0)		if (clrcmp(maps[now->x + XSIZE * (now->y - 1)], gray)) queuepro(now->x, now->y - 1, true);
		}
		now = queuepro(0, 0, false);	/* �L���[��1���炷 */
	} while(now != NULL);

	if (kill == false)
	{
		/* �t�@�C���������ݏ��� ���U�őł��~�߂������Ɏc�����O���C�������h�� */
		if ((fp = fopen(file, "w")) == NULL) return false;

		/*fprintf(fp, "P6\n# 10RU004 �΋��ˑ�\n%d %d\n255 ", XSIZE, YSIZE);

		for(countT = 0; countT < XSIZE * YSIZE; countT++)
			if (maps[countT].r != maps[countT].g || maps[countT].g != maps[countT].b)
				 fprintf(fp, "%c%c%c", maps[countT].r, maps[countT].g, maps[countT].b);
			else fprintf(fp, "%c%c%c", 255, 255, 255);*/

		fprintf(fp, "P3\n# 10RU004 �΋��ˑ�\n%d %d\n255\n", XSIZE, YSIZE);

		for(countZ = 0; countZ < YSIZE; countZ++)
		{
			for(countT = 0; countT < XSIZE; countT++)
				if (maps[countT + countZ * XSIZE].r != maps[countT + countZ * XSIZE].g || maps[countT + countZ * XSIZE].g != maps[countT + countZ * XSIZE].b)
					 fprintf(fp, "%03d %03d %03d ", maps[countT + countZ * XSIZE].r, maps[countT + countZ * XSIZE].g, maps[countT + countZ * XSIZE].b);
				else fprintf(fp, "%03d %03d %03d ", 255, 255, 255);

			fprintf(fp, "\n");
		}
		fclose(fp);
	}
	free(maps);

	return true;
}
