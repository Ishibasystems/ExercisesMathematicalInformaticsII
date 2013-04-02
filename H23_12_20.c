/* 数理情報学演習II H23_12_20.c */
/* マンデルブロ 標準偏差+色相回転+出力メニュー */
/* 2011.12.20, 10RU004, 石橋祥太 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define TMAX 1000
#define XSIZE 768
#define YSIZE 768

long input(char *, long);
char Mandelbrot(char *, char, char, double, double, double, double, double, double, double);

void main(void)
{
	char mjmode, colmode, file[256] = "\0", buf[256], count;
	double XMIN, XMAX, YMIN, YMAX, A = 0.2, B = -0.52, hoge;

	time_t timer;

	printf("図形選択\n1:\tマンデルブロ集合\n2:\tジュリア集合\nOther:\t終了\n>> ");
	mjmode = input(buf, 256);
	if (mjmode < 1 || mjmode > 2) return;

	printf("\n色量モード選択\n1:\t変化量\n2:\t標準偏差\nOther:\t終了\n>> ");
	colmode = input(buf, 256);
	if (colmode < 1 || colmode > 2) return;

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

	if (Mandelbrot(file, mjmode, colmode, 0.2, -0.52, 2.0, XMIN, XMAX, YMIN, YMAX) != 0)
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

char Mandelbrot(char *file, char mjmode, char colmode, double A, double B, double EP, double XMIN, double XMAX, double YMIN, double YMAX)
{
	char latch;
	int countX, countY, countZ, countT;
	double x, y, xt, yt, xt_, yt_, zt[TMAX],
		sT, fT, flag, ZMIN = DBL_MAX, ZMAX = DBL_MIN, inf = log(0);

	FILE *fp;

	if ((fp = fopen(file, "w")) == NULL) return 1;
	fprintf(fp, "P3\n# 10RU004 石橋祥太\n%d %d\n255\n", XSIZE, YSIZE);

	for(latch = 0; latch < 2; latch++)
	{
		for(countY = 0; countY < YSIZE; countY++)
		{
			y = (YMAX - YMIN) * countY / YSIZE + YMIN;

			for(countX = 0; countX < XSIZE; countX++)
			{
				x = (XMAX - XMIN) * countX / XSIZE + XMIN;

				switch(mjmode)
				{
				case 1:
					xt_ = yt_ = 0.0;
					A = x;
					B = y;
					break;

				case 2:
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
							case 1:
								flag = zt[countZ] - zt[countZ - 1];
								break;

							case 2:
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
						if (latch == 0)
						{
							if (ZMIN > flag) ZMIN = flag;
							if (ZMAX < flag) ZMAX = flag;
						}
						break;
					}
				}

				if (latch != 0)
				{
					if (flag == inf) fprintf(fp, "255 255 255 ");
					else
					{
						flag = (M_PI * 330 / 180 * (flag - ZMIN) / (ZMAX - ZMIN));
						fprintf(fp, "%d %d %d ",
							(int)(255 * (cos(flag				) + 1) / 2),
							(int)(255 * (cos(flag - M_PI * 2 / 3) + 1) / 2),
							(int)(255 * (cos(flag - M_PI * 4 / 3) + 1) / 2));
					}
				}
			}
		}
	}
	fclose(fp);
	return 0;
}
