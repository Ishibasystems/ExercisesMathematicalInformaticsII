/* 数理情報学演習II H23_11_22_212.c */
/* マンデルブロ 標準偏差+? */
/* 2011.11.22, 10RU004, 石橋祥太 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TMAX 1000
#define XSIZE 900
#define YSIZE 900

void main(void)
{
	int countX, countY, countZ, countT, flag;
	double x, y, xt[TMAX + 1], yt[TMAX + 1], zt[TMAX], XMIN = -2.4, XMAX = 2.4, YMIN = -2.4, YMAX = 2.4;
	double A = 0.2, B = -0.52, EP = 2.0, sT, fT;

	FILE *fp;
	
	fp = fopen("test.pgm", "w");

	fprintf(fp, "P2\n");
	fprintf(fp, "# 10RU004 石橋祥太\n");
	fprintf(fp, "%d %d\n", XSIZE, YSIZE);
	fprintf(fp, "255\n");

	for(countY = 0; countY < YSIZE; countY++)
	{
		y = (YMAX - YMIN) * countY / YSIZE + YMIN;
		for(countX = 0; countX < XSIZE; countX++)
		{
			x = (XMAX - XMIN) * countX / XSIZE + XMIN;

			xt[0] = 0.0;
			yt[0] = 0.0;
			A = x;
			B = y;
			flag = 255;

			for(countZ = 0; countZ < TMAX; countZ++)
			{
				xt[countZ + 1] = xt[countZ] * xt[countZ] - yt[countZ] * yt[countZ] + A;
				yt[countZ + 1] = xt[countZ] * yt[countZ] * 2 + B;
				zt[countZ] = sqrt(xt[countZ + 1] * xt[countZ + 1] + yt[countZ + 1] * yt[countZ + 1]);
				if (zt[countZ] > EP)
				{
					sT = fT = 0.0;
					for(countT = 0; countT < countZ; countT++) sT += zt[countT];
					sT /= countT;
					for(countT = 0; countT < countZ; countT++)
					{
						fT += (zt[countT] - sT) * (zt[countT] - sT);
					}
					flag = (int)exp(fT / countZ +1) * 255;
					break;
				}
			}
			fprintf(fp, "%d ", flag);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	return;
}