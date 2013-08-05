/* 数理情報学演習II H23_11_22_212c.c */
/* マンデルブロ 標準偏差+色明暗 */
/* 2011.11.28, 10RU004, 石橋祥太 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#define TMAX 1000
#define XSIZE 900
#define YSIZE 900

void main(void)
{
	char latch;
	int countX, countY, countZ, countT;
	double x, y, xt[TMAX + 1], yt[TMAX + 1], zt[TMAX], XMIN = -1.6-0.53, XMAX = 1.6-0.53, YMIN = -1.6, YMAX = 1.6,
		A = 0.2, B = -0.52, EP = 2.0, sT, fT, flag, ZMIN = DBL_MAX, ZMAX = DBL_MIN, inf = -log(0);

	FILE *fp;
	if ((fp = fopen("output.ppm", "w")) == NULL) exit(0);
	fprintf(fp, "P3\n# 10RU004 石橋祥太\n%d %d\n255\n", XSIZE, YSIZE);

	for(latch = 0; latch < 2; latch++)
	{
		for(countY = 0; countY < YSIZE; countY++)
		{
			y = (YMAX - YMIN) * countY / YSIZE + YMIN;

			for(countX = 0; countX < XSIZE; countX++)
			{
				x = (XMAX - XMIN) * countX / XSIZE + XMIN;

				xt[0] = yt[0] = 0.0;
				A = x;
				B = y;
				flag = inf;

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
						flag = sqrt(fT / countZ);
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
						countZ = (int)(1791 / (ZMAX - ZMIN) * (flag - ZMIN)) + 256;	/* ヒストグラム領域の変更式をここで導入（笑）*/
						fprintf(fp, "%d %d %d ",
							countZ % 256 * (((countZ >> 8) & 1) != 0),
							countZ % 256 * (((countZ >> 8) & 2) != 0),
							countZ % 256 * (((countZ >> 8) & 4) != 0));
						// 色順番 赤緑黄青紫水灰(暗→明) 全1792階調
					}
				}
			}
		}
	}
	fclose(fp);

	return;
}
