/* 数理情報学演習II H24_01_13.c */
/* マンデルブロ 出力メニュー */
/* 2012.01.15, 10RU004, 石橋祥太 */

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

	Mandelbrot(NULL, 0, 0, 0, 0, 0, 0, 0, 0, (unsigned short)XSIZE, (unsigned short)YSIZE);
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
	Mandelbrot(file, 1 | mjmode << 1 | colmode << 3 | 1 << 5 | 1 << 7, A, B,
				2.0, XMIN, XMAX, -YMAX, -YMIN, (unsigned short)XSIZE, (unsigned short)YSIZE);
	printf("出力中(2/2)... しばらくお待ちください\n", file);
	if (Mandelbrot(file, 1 | mjmode << 1 | colmode << 3 | bmpmode << 6 | 1 << 7, A, B,
					2.0, XMIN, XMAX, -YMAX, -YMIN, (unsigned short)XSIZE, (unsigned short)YSIZE) != 0)
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
