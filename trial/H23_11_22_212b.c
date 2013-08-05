/* 数理情報学演習II H23_11_22_212b.c */
/* マンデルブロ 標準偏差+画像マップ保持 */
/* 2011.11.28, 10RU004, 石橋祥太 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define TMAX 1000
#define XSIZE 900
#define YSIZE 900
#define NUMMAX 255

void main(void)
{
	char latch;
	int countX, countY, countZ, countT;
	double x, y, xt[TMAX + 1], yt[TMAX + 1], zt[TMAX], *maps, XMIN = -1.6-0.53, XMAX = 1.6-0.53, YMIN = -1.6, YMAX = 1.6,
		A = 0.2, B = -0.52, EP = 2.0, sT, fT, ZMIN = DBL_MAX, ZMAX = DBL_MIN, inf = -log(0);

	FILE *fp;

	if ((fp = fopen("output.pgm", "w")) == NULL) exit(0);

	if ((maps = malloc(sizeof(double) * XSIZE * YSIZE)) == NULL) exit(0);

	fprintf(fp, "P2\n# 10RU004 石橋祥太\n%d %d\n%d\n", XSIZE, YSIZE, NUMMAX);

	for(latch = 0; latch < 2; latch++)
	{
		for(countY = 0; countY < YSIZE; countY++)
		{
			y = (YMAX - YMIN) * countY / YSIZE + YMIN;

			for(countX = 0; countX < XSIZE; countX++)
			{
				x = (XMAX - XMIN) * countX / XSIZE + XMIN;

				if (latch == 0)
				{
					xt[0] = yt[0] = 0.0;
					A = x;
					B = y;
					*(maps + countX + XSIZE * countY) = inf;

					for(countZ = 0; countZ < TMAX; countZ++)
					{
						xt[countZ + 1] = xt[countZ] * xt[countZ] - yt[countZ] * yt[countZ] + A;
						yt[countZ + 1] = xt[countZ] * yt[countZ] * 2 + B;
						zt[countZ] = sqrt(xt[countZ + 1] * xt[countZ + 1] + yt[countZ + 1] * yt[countZ + 1]);
						if (zt[countZ] > EP)
						{
							sT = fT = 0.0;
							for(countT = 0; countT < countZ; sT += zt[countT++]);
							sT /= countT;
							for(countT = 0; countT < countZ; fT += (zt[countT] - sT) * (zt[countT++] - sT));
							*(maps + countX + XSIZE * countY) = fT = sqrt(fT / countZ);;
							if (ZMIN > fT) ZMIN = fT;
							if (ZMAX < fT) ZMAX = fT;
							break;
						}
					}
				}
				else
				{
					if (*(maps + countX + XSIZE * countY) == inf) fprintf(fp, "%d ", NUMMAX);
					else fprintf(fp, "%d ", (int)(NUMMAX / (ZMAX - ZMIN) * (*(maps + countX + XSIZE * countY) - ZMIN)));	/* ヒストグラム領域の変更式をここで導入（笑）*/
				}
			}
		}
	}
	free(maps);
	fclose(fp);

	return;
}
