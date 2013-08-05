/* 数理情報学演習II H24_01_10.c */
/* マンデルブロ 標準偏差+色相回転+出力メニュー */
/* 2012.01.10, 10RU004, 石橋祥太 */

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

	printf("図形選択\n1:\tマンデルブロ集合\n2:\tジュリア集合\nOther:\t終了\n>> ");
	mjmode = input(buf, 256) - 1;
	if (mjmode < 0 || mjmode > 1) return;

	printf("\n色量モード選択\n1:\t変化量\n2:\t標準偏差\nOther:\t終了\n>> ");
	colmode = input(buf, 256) - 1;
	if (colmode < 0 || colmode > 1) return;

	printf("\n描画範囲を入力(X軸)\n例) -2.13,1.07\n>> ");
	input(buf, 256);
	sscanf(buf, "%lf,%lf%*[^\n]", &XMIN, &XMAX);
	if (XMIN > XMAX) return;

	printf("\n描画範囲を入力(Y軸)\n例) -1.6,1.6\n>> ");
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

	printf("\n出力ファイル名を入力(.ppm自動補間)\n>> ");
	input(file, 256);

	for(count = 0; count < strlen(file); buf[count++] = tolower(file[count]));	/* fileをbufにコピーしつつ大文字を見つけ次第小文字に */

	buf[count] = '\0';

	if (count < 5 || strcmp(&buf[count - 4], ".ppm") != 0)
	{
		if (count == 0)
		{
			time(&timer);
			sprintf(buf, "%s", ctime(&timer));
			for(count = 0; count < strlen(buf); count++)
				if (buf[count] == ':' || buf[count] == '\n') buf[count] = '_';	/* ctime関数で返す文字列中のファイル名に指定できない文字を置換 */
		}
		sprintf(file, "%s.ppm", buf);
	}

	printf("\n出力ファイル名 : %s\n出力中... しばらくお待ちください\n", file);

	Mandelbrot(NULL, false, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
	/* 第二引数メモ：
		1ビット目 Min-Max引継ぎフラグ(falseで初期化)
		2ビット目 図形選択
		3ビット目 色量モード
		4ビット目 ファイル書込なし(Min-Maxの設定用)
	*/

	Mandelbrot(file, true | mjmode << 1 | colmode << 2 | 1 << 3, 0.2, -0.52, 2.0, XMIN, XMAX, YMIN, YMAX);
	if (Mandelbrot(file, true | mjmode << 1 | colmode << 2, 0.2, -0.52, 2.0, XMIN, XMAX, YMIN, YMAX) == false)
		 printf("エラーが発生しました\n");
	else printf("出力が終了しました\n");

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
	/* キューポインタ管理 */
	static queue *head = NULL;
	static queue *tail = NULL;
	queue *q = head;

	switch(mode)
	{
	case true:
		while(q != NULL)
		{
			if (x == q->x && y  == q->y) return NULL;	/* キューに登録済み(座標が同じ) */
			q = q->p;
		}
		if ((q = malloc(sizeof(queue))) == NULL) exit(1);	/* そこまで面倒見れません */
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
		break;

	default:
		break;
	}
	return head;
}

char Mandelbrot(char *file, unsigned char mode, double A, double B, double EP, double XMIN, double XMAX, double YMIN, double YMAX)
{
	char latch, mjmode = (mode & 2) != 0, colmode = (mode & 4) != 0, kill = (mode & 8) != 0;
	long long countX, countY, countZ, countT;
	double x, y, xt, yt, xt_, yt_, zt[TMAX], sT, fT, flag, inf = log(0);
	static double ZMIN = DBL_MAX, ZMAX = DBL_MIN;
	color *maps, gray = {128, 128, 128};
	queue *now;

	FILE *fp;

	if ((mode & 1) == false)
	{
		ZMIN = DBL_MAX;
		ZMAX = DBL_MIN;
		return true;
	}

	if ((maps = malloc(sizeof(color) * XSIZE * YSIZE)) == NULL) return false;

	for(countT = 0; countT < XSIZE * YSIZE; countT++) maps[countT] = gray;

	now = queuepro(0, 0, true);
	/*for(latch = 0; latch + kill < 2; latch++)
	{
		for(countY = 0; countY < YSIZE; countY++)
		{
			for(countX = 0; countX < XSIZE; countX++)
			{
	*/
	do {
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

		flag = inf;

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
						/* 最終変化量 */
						flag = zt[countZ] - zt[countZ - 1];
						break;

					case true:
						/* 標準偏差 */
						countZ++;
						sT = fT = 0.0;
						for(countT = 0; countT < countZ; sT += zt[countT++]);
						sT /= countZ;
						for(countT = 0; countT < countZ; fT += (zt[countT] - sT) * (zt[countT++] - sT));
						flag = sqrt(fT / countZ);
						break;

					default:
						break;
				}
				if (kill)
				{
					/* Min-Maxの設定 */
					if (ZMIN > flag) ZMIN = flag;
					if (ZMAX < flag) ZMAX = flag;
				}
				break;
			}
		}

		if (flag * 0 != 0)
			/* 発散したので白塗り */
			  maps[now->x + XSIZE * now->y].r
			= maps[now->x + XSIZE * now->y].g
			= maps[now->x + XSIZE * now->y].b
			= 255;
		else
		{
			/* Min-Maxの設定時は最低限判定できる色にする 時間がかかる実数演算は避ける */
			if (kill) maps[now->x + XSIZE * now->y].g = 255;
			else
			{
				/* 色づけ */
				flag = (M_PI * 330 / 180 * (flag - ZMIN) / (ZMAX - ZMIN));
				maps[now->x + XSIZE * now->y].r = (unsigned char)(255 * (cos(flag				) + 1) / 2);
				maps[now->x + XSIZE * now->y].g = (unsigned char)(255 * (cos(flag - M_PI * 2 / 3) + 1) / 2);
				maps[now->x + XSIZE * now->y].b = (unsigned char)(255 * (cos(flag - M_PI * 4 / 3) + 1) / 2);
			}
			/* 発散しなかったのでキュー追加処理 (縁越えのバグあり) */
			if (now->x < XSIZE)	if (clrcmp(maps[(now->x + 1) + XSIZE * now->y], gray)) queuepro(now->x + 1, now->y, true);
			if (now->x > 0)		if (clrcmp(maps[(now->x - 1) + XSIZE * now->y], gray)) queuepro(now->x - 1, now->y, true);
			if (now->y < YSIZE)	if (clrcmp(maps[now->x + XSIZE * (now->y + 1)], gray)) queuepro(now->x, now->y + 1, true);
			if (now->y > 0)		if (clrcmp(maps[now->x + XSIZE * (now->y - 1)], gray)) queuepro(now->x, now->y - 1, true);
		}
		now = queuepro(0, 0, false);	/* キューを1減らす */
	} while(now != NULL);

	if (kill == false)
	{
		/* ファイル書き込み処理 発散で打ち止めた内側に残ったグレイも白く塗る */
		if ((fp = fopen(file, "w")) == NULL) return false;
		fprintf(fp, "P6\n# 10RU004 石橋祥太\n%d %d\n255 ", XSIZE, YSIZE);

		for(countT = 0; countT < XSIZE * YSIZE; countT++)
			if (maps[countT].r == maps[countT].g && maps[countT].g == maps[countT].b)
				fprintf(fp, "%c%c%c", 255, 255, 255);
			else fprintf(fp, "%c%c%c", maps[countT].r, maps[countT].g, maps[countT].b);
		fclose(fp);
	}
	free(maps);
	printf("%f\n%f\n\n", ZMAX, ZMIN);
	return true;
}
