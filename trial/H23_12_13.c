/* 数理情報学演習II H23_12_13.c */
/* マンデルブロ 標準偏差+色相回転+出力メニュー */
/* 2011.11.28, 10RU004, 石橋祥太 */

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

void main(void)
{
	char latch, mjmode, colmode, file[256] = "output.ppm";
	int countX, countY, countZ, countT;
	double x, y, xt[TMAX + 1], yt[TMAX + 1], zt[TMAX], XMIN = -1.6-0.53, XMAX = 1.6-0.53, YMIN = -1.6, YMAX = 1.6,
		A = 0.2, B = -0.52, EP = 2.0, sT, fT, flag, ZMIN = DBL_MAX, ZMAX = DBL_MIN, inf = -log(0);

	FILE *fp;
	time_t timer;

	printf("図形選択\n1:\tマンデルブロ集合\n2:\tジュリア集合\nOther:\t終了\n>> ");
	mjmode = input(file, 256);
	if (mjmode < 1 || mjmode > 2) return;

	printf("\n色量モード選択\n1:\t変化量\n2:\t標準偏差\nOther:\t終了\n>> ");
	colmode = input(file, 256);
	if (colmode < 1 || colmode > 2) return;

	printf("\n描画範囲を入力(X軸)\n例) -2.13,1.07\n>> ");
	input(file, 256);
	sscanf(file, "%lf,%lf%*[^\n]", &XMIN, &XMAX);
	if (XMIN > XMAX) return;

	printf("\n描画範囲を入力(Y軸)\n例) -1.6,1.6\n>> ");
	input(file, 256);
	sscanf(file, "%lf,%lf%*[^\n]", &YMIN, &YMAX);
	if (YMIN > YMAX) return;

	if (YSIZE * (XMAX - XMIN) < XSIZE * (YMAX - YMIN))
	{
		flag = (XSIZE * (YMAX - YMIN) / YSIZE - XMAX + XMIN) / 2;
		XMAX += flag;
		XMIN -= flag;
	}
	else
	{
		flag = (YSIZE * (XMAX - XMIN) / XSIZE - YMAX + YMIN) / 2;
		YMAX += flag;
		YMIN -= flag;
	}

	for(countY = 0; countY < 256; file[countY++] = '\0');
	printf("\n出力ファイル名を入力(.ppm自動補間)\n>> ");
	input(file, 256);
	file[255] = '\0';	/* 安全Null */

	printf("Hello, %s\n", file);

	for(countY = 0; countY < 252; countY++)
		if (file[countY] == '\0')
		{
			if (countY < 5 || file[countY - 3] != '.'
				|| (file[countY - 3] != 'p' && file[countY - 3] != 'P')
				|| (file[countY - 2] != 'p' && file[countY - 2] != 'P')
				|| (file[countY - 1] != 'm' && file[countY - 1] != 'M'))
			{
				if (countY == 0)
				{
					time(&timer);
					sprintf(file, "%s.ppm", ctime(&timer));
					for(countY = 0; file[countY] != '\0'; countY++)
						if (file[countY] == ':' || file[countY] == '\r' || file[countY] == '\n')
							file[countY] = '_';
				}
				else
				{
					sprintf(file, "%s.ppm", file);
				}
			}
			break;
		}

	printf("Hello, %s\n", file);
	printf("\n出力中... しばらくお待ちください\n");

	if ((fp = fopen(file, "w")) == NULL)
	{
		printf("ファイルのオープンに失敗しました\n");
		exit(0);
	}
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
					xt[0] = yt[0] = 0.0;
					A = x;
					B = y;
					break;

				case 2:
					xt[0] = x;
					yt[0] = y;
					break;

				default:
					break;
				}

				flag = inf;

				for(countZ = 0; countZ < TMAX; countZ++)
				{
					xt[countZ + 1] = xt[countZ] * xt[countZ] - yt[countZ] * yt[countZ] + A;
					yt[countZ + 1] = xt[countZ] * yt[countZ] * 2 + B;
					zt[countZ] = sqrt(xt[countZ + 1] * xt[countZ + 1] + yt[countZ + 1] * yt[countZ + 1]);
					if (zt[countZ] > EP)
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
	printf("出力が終了しました\n");
	return;
}

long input(char s[], long len)
{
	fgets(s, len, stdin);

	if (strchr(s, '\n') != 0) s[strlen(s) - 1] = '\0';
	else while(getchar() != '\n');

	return atol(s);
}
