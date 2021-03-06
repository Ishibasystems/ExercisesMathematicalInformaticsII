/* 数理情報学演習II H24_01_13.c */
/* マンデルブロ 出力メニュー */
/* 2012.01.16, 10RU004, 石橋祥太 */

#include <time.h>
#include "Mandelbrot.h"

long input(char *, long);

void main(void)
{
	char mjmode, colmode, bmpmode, buf[256], file[256] = "\0", type[2][4] = {"ppm", "bmp"};
	unsigned char count;
	unsigned short XSIZE, YSIZE;
	double XMIN = -1.670322, XMAX = -1.581711, YMIN = -0.067780, YMAX = 0.020830, A = 0.2, B = -0.52, hoge;

	time_t timer;

	//printf("ファイルタイプ\n1:\tPPM\n2:\tBMP\nOther:\t終了\n>> ");
	bmpmode = 1;

	YSIZE = XSIZE = 768;

	//printf("\n図形選択\n1:\tマンデルブロ集合\n2:\tバーニングシップ集合\n3:\tトライコーン集合\nOther:\t終了\n>> ");
	mjmode = 1;

	//printf("\n色量モード選択\n1:\t変化量\n2:\t標本分散\n3:\t標準偏差\n4:\t発散に要した回数の逆数\nOther:\t終了\n>> ");
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
	printf("描画範囲:\nX %f to %f\nY %f to %f\nが指定されました\n", XMIN, XMAX, YMIN, YMAX);

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
				if (buf[count] == ':') buf[count] = '_';	/* ctime関数で返す文字列中のファイル名に指定できない文字を置換 */
				if (buf[count] == '\n') buf[count] = '\0';
			}
		}
		sprintf(file, "%s.%s", buf, type[bmpmode]);
	}

	printf("\n出力ファイル名 : \"%s\"\n出力中(1/2)... しばらくお待ちください\n", file);

	Mandelbrot(NULL, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0);
	/* 第二引数メモ：
		00	1ビット目 図形選択
		01	2ビット目 図形選択
		02	Xビット目 図形選択(予約)
		03	9ビット目 図形選択(true:ジュリア、false:normal)
		04	8ビット目 色モード(true:color、false:gray)
		05	3ビット目 色量モード
		06	4ビット目 色量モード
		07	Xビット目 色量モード(予約)
		08	5ビット目 ファイル書込なし(Min-Maxの設定用)
		09	6ビット目 ファイルモード(true:BMP、false:PPM)
		10	Xビット目 (予約)
		11	Xビット目 (予約)
		12	7ビット目 詳細検索(true:四方八方、false:四方)
		13	Xビット目 (予約)
		14	Xビット目 (予約)
		15	Xビット目 (予約)
		-0ビット目 Min-Max引継ぎフラグ(falseで初期化)
	*/
	Mandelbrot(file, mjmode | colmode << 5 | 1 << 8 | 1 << 12, A, B,
				2.0, 2.0, XMIN, XMAX, YMIN, YMAX, XSIZE, YSIZE);
	printf("出力中(2/2)... しばらくお待ちください\n", file);
	if (Mandelbrot(file, mjmode | colmode << 5 | bmpmode << 9 | 1 << 12, A, B,
					2.0, 2.0, XMIN, XMAX, YMIN, YMAX, XSIZE, YSIZE) != 0)
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
