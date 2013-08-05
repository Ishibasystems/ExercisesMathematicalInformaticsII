/* 数理情報学演習II H24_01_13.c */
/* マンデルブロ 標準偏差+色相回転+出力メニュー+画像マップ保持 */
/* 2012.01.13, 10RU004, 石橋祥太 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795L	/* VC++6.0に定義されてなかった */
#endif

#define true 1
#define false 0
#define TMAX 1000

typedef struct{
	/* BMPフォーマット順 */
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

	printf("ファイルタイプ\n1:\tPPM\n2:\tBMP\nOther:\t終了\n>> ");
	bmpmode = input(buf, 256) - 1;
	if (bmpmode < 0 || bmpmode > 1) return;

	printf("\n描画サイズを入力(%s)\n0以下:\t終了\n例) 768,768\n>> ", type[bmpmode]);
	input(buf, 256);
	sscanf(buf, "%d,%d%*[^\n]", &XSIZE, &YSIZE);
	if (XSIZE <= 0) return;
	if (YSIZE <= 0) return;

	printf("\n図形選択\n1:\tマンデルブロ集合\n2:\tジュリア集合\n3:\tバーニングシップ集合\nOther:\t終了\n>> ");
	mjmode = input(buf, 256) - 1;
	if (mjmode < 0 || mjmode > 3) return;

	if (mjmode & 1)
	{
		printf("\nジュリア集合:A,B初期値を入力\n例) 0.2,-0.52\n>> ");
		input(buf, 256);
		sscanf(buf, "%lf,%lf%*[^\n]", &A, &B);
	}

	printf("\n色量モード選択\n1:\t変化量\n2:\t標本分散\n3:\t標準偏差\n4:\t発散に要した回数\nOther:\t終了\n>> ");
	colmode = input(buf, 256) - 1;
	if (colmode < 0 || colmode > 3) return;

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
	printf("\n描画範囲:\nX %f to %f\nY %f to %f\nが指定されました\n", XMIN, XMAX, YMIN, YMAX);

	printf("\n出力ファイル名を入力(.%s自動補間)\n>> ", type[bmpmode]);
	input(file, 256);

	for(count = 0; count < strlen(file); buf[count++] = tolower(file[count]));	/* fileをbufにコピーしつつ大文字を見つけ次第小文字に */

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
				if (buf[count] == ':') buf[count] = '_';	/* ctime関数で返す文字列中のファイル名に指定できない文字を置換 */
				if (buf[count] == '\n') buf[count] = '\0';
			}
		}
		sprintf(file, "%s.%s", buf, type[bmpmode]);
	}

	printf("\n出力ファイル名 : \"%s\"\n出力中(1/2)... しばらくお待ちください\n", file);

	Mandelbrot(NULL, false, 0, 0, 0, 0, 0, 0, 0, (unsigned short)XSIZE, (unsigned short)YSIZE);
	/* 第二引数メモ：
		0ビット目 Min-Max引継ぎフラグ(falseで初期化)
		1ビット目 図形選択
		2ビット目 図形選択
		3ビット目 色量モード
		4ビット目 色量モード
		5ビット目 ファイル書込なし(Min-Maxの設定用)
		6ビット目 ファイルモード(true:BMP、false:PPM)
		7ビット目 詳細検索(true:四方八方、false:四方)
	*/
	Mandelbrot(file, true | mjmode << 1 | colmode << 3 | 1 << 5, 0.2,
				-0.52, 2.0, XMIN, XMAX, -YMAX, -YMIN, (unsigned short)XSIZE, (unsigned short)YSIZE);
	printf("出力中(2/2)... しばらくお待ちください\n", file);
	if (Mandelbrot(file, true | mjmode << 1 | colmode << 3 | bmpmode << 6 | 1 << 7, 0.2,
					-0.52, 2.0, XMIN, XMAX, -YMAX, -YMIN, (unsigned short)XSIZE, (unsigned short)YSIZE) != 0)
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

queue *queuepro(unsigned short x, unsigned short y, color *maps)
{
	/* キューポインタ管理 もはや使い回せないほどに特化 */
	static queue *head = NULL, *tail = NULL;
	static unsigned short XSIZE = 0, YSIZE = 0;
	queue *q = head;

	if (maps != NULL)
	{
		/* キューを登録*/
		if (x < 0 || y < 0 || x > XSIZE - 1 || y > YSIZE - 1
			|| XSIZE == 0 || YSIZE == 0) return head;	/* マップサイズ外、マップサイズ設定がない場合 */

		if (maps[x + XSIZE * y].r != 128
			|| maps[x + XSIZE * y].g != 128
			|| maps[x + XSIZE * y].b != 128)
			return head;	/* グレイ(未計算点)でなかった */

		while(q != NULL)
		{
			if (x == q->x && y == q->y) return head;	/* キューに登録済み(座標が同じ) */
			q = q->p;
		}
		if ((q = malloc(sizeof(queue))) == NULL) exit(1);	/* そこまで面倒見れません */
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
			/* キューを削除*/
			q = head->p;
			if (q == NULL) tail = NULL;	/* このキューの後ろがない */
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

	if ((mode & 1) == false)	/* Min-Maxの初期化 初回実行不要 */
	{
		ZMIN =  DBL_MAX;
		ZMAX = -DBL_MAX;
		return 0;
	}

	if ((maps = malloc(sizeof(color) * XSIZE * YSIZE)) == NULL) return 1;	/* フラグ兼色マップ確保 */

	/* マップをグレイ(未計算点フラグ)で塗りつぶし */
	for(count = 0; count < XSIZE * YSIZE; count++) maps[count] = gray;

	now = queuepro(XSIZE, YSIZE, NULL);	/* キューの振り分け条件登録 */

	for(count = 0; count < XSIZE; count++)	/* 初期キューを登録 上下辺の点全て */
	{
		queuepro(count, 0		 , maps);
		queuepro(count, YSIZE - 1, maps);
	}

	for(count = 0; count < YSIZE; count++)	/* 初期キューを登録 左右辺の点全て */
	{
		queuepro(0		  , count, maps);
		queuepro(XSIZE - 1, count, maps);
	}

	now = queuepro(0, 0, maps);	/* キューのヘッダのポインタ取得 */

	while(now != NULL)	/* キューがなくなるまで繰り返す */
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
							/* 最終変化量 */
							inf = (zt[countZ - 0] - zt[countZ - 1]) / (countZ + 1);
							break;

						case 1:
							/* 標本分散 */
							countZ++;
							sT = fT = 0.0;
							for(countT = 0; countT < countZ; sT += zt[countT++]);
							sT /= countZ;
							for(countT = 0; countT < countZ; fT += (zt[countT] - sT) * (zt[countT++] - sT));
							inf = fT / countZ;
							break;

						case 2:
							/* 標準偏差 */
							countZ++;
							sT = fT = 0.0;
							for(countT = 0; countT < countZ; sT += zt[countT++]);
							sT /= countZ;
							for(countT = 0; countT < countZ; fT += (zt[countT] - sT) * (zt[countT++] - sT));
							inf = sqrt(fT / countZ);
							break;

						case 3:
							/* 発散に要した回数 */
							inf = countZ;
							break;
					}

					if (kill)
					{
						/* Min-Maxの設定 */
						if (ZMIN > inf) ZMIN = inf;
						if (ZMAX < inf) ZMAX = inf;
					}

					flag = false;
					break;
				}
			}

			if (flag) maps[now->x + XSIZE * now->y] = white;	/* 発散したので白塗り */
			else
			{
				/* Min-Maxの設定時は最低限判定できる色(グレースケール以外)にする 時間がかかる実数演算は避ける */
				if (kill) maps[now->x + XSIZE * now->y].g = 255;
				else
				{
					/* 色づけ */
					inf = (M_PI * 330 / 180 * (inf - ZMIN) / (ZMAX - ZMIN));
					maps[now->x + XSIZE * now->y].r = (unsigned char)(255 * (cos(inf			   ) + 1) / 2);
					maps[now->x + XSIZE * now->y].g = (unsigned char)(255 * (cos(inf + M_PI * 2 / 3) + 1) / 2);
					maps[now->x + XSIZE * now->y].b = (unsigned char)(255 * (cos(inf + M_PI * 4 / 3) + 1) / 2);
				}
				/* 発散しなかったのでキュー追加処理 グレイ(未計算点フラグ)なら追加 四方(簡易) */
				queuepro(now->x + 1, now->y, maps);
				queuepro(now->x - 1, now->y, maps);
				queuepro(now->x, now->y + 1, maps);
				queuepro(now->x, now->y - 1, maps);
				if (latch)	/*  八方(詳細) */
				{
					queuepro(now->x + 1, now->y + 1, maps);
					queuepro(now->x - 1, now->y + 1, maps);
					queuepro(now->x + 1, now->y - 1, maps);
					queuepro(now->x - 1, now->y - 1, maps);
				}
			}
		}
		now = queuepro(XSIZE, YSIZE, false);	/* キューを1減らす(開放) */
	}

	if (kill == false)
	{
		/* ファイル書き込み処理 */
		if ((fp = fopen(file, "wb")) == NULL) return 2;

		if (bmp)
		{
			/* BITMAP出力 構造体のアライメント回避が(クロスプラットフォームだとコンパイラの指定変更とか)
			   面倒なので非構造体で書き出し */

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
			for(countZ = YSIZE; countZ > 0; countZ--)	/* 左下から右上へ */
			{
				for(count = countZ * XSIZE - XSIZE; count < XSIZE + countZ * XSIZE - XSIZE; count++)
					/* 発散で打ち止めた内側に残ったグレイは白く塗る */
					if (clrcmp(maps[count], gray))
						fprintf(fp, "%c%c%c", 255, 255, 255);
					else fwrite(&maps[count], sizeof(color), 1, fp);

				for(count = 0; count < XSIZE * 3 % 4; count++)
					fprintf(fp, "%c", 255);	/* 1ラインは必ず4バイトの倍数に */
			}
		}
		else
		{
			/* P3出力 */
			fprintf(fp, "P3\n# 10RU004 石橋祥太\n%d %d\n255\n", XSIZE, YSIZE);

			for(count = 0; count < XSIZE * YSIZE; count++)
				/* 発散で打ち止めた内側に残ったグレイは白く塗る */
				if (clrcmp(maps[count], gray)) fprintf(fp, "%03d %03d %03d ", 255, 255, 255);
				else fprintf(fp, "%03d %03d %03d ", maps[count].r, maps[count].g, maps[count].b);

			/* P6出力 なぜかずれる(PPMバイナリフォーマットの理解できてない? IrfanView側の問題?)ので放置（ぉぃ */
			/*fprintf(fp, "P6\n# 10RU004 石橋祥太\n%d %d\n255\n", XSIZE, YSIZE);

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
