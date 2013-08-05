/* 数理情報学演習II H23_11_22_212d.c */
/* マンデルブロ 標準偏差+色相回転 */
/* 2011.11.28, 10RU004, 石橋祥太 */

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define TMAX 1000
#define XSIZE 768
#define YSIZE 768

void main(void)
{
	char latch;
	int countX, countY, countZ, countT;
	double x, y, xt[TMAX + 1], yt[TMAX + 1], zt[TMAX], XMIN = -1.6-0.53, XMAX = 1.6-0.53, YMIN = -1.6, YMAX = 1.6,
		A = 0.2, B = -0.52, EP = 2.0, sT, fT, flag, ZMIN = DBL_MAX, ZMAX = DBL_MIN, inf = -log(0);

	FILE *fp;
	if ((fp = fopen("output2.ppm", "w")) == NULL) exit(0);
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
						countZ++;
						sT = fT = 0.0;
						for(countT = 0; countT < countZ; sT += zt[countT++]);
						sT /= countZ;
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
						flag = (M_PI * 2 * (flag - ZMIN) / (ZMAX - ZMIN));	/* ヒストグラム領域の変更式をここで導入（笑）*/
						fprintf(fp, "%d %d %d ",
							(int)(255 * (cos(flag				) + 1) / 2),
							(int)(255 * (cos(flag - M_PI * 2 / 3) + 1) / 2),
							(int)(255 * (cos(flag - M_PI * 4 / 3) + 1) / 2));
						// 色順番 赤黄緑水青紫 全16777216階調
					}
				}
			}
		}
	}
	fclose(fp);

	return;
}
