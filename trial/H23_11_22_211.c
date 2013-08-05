/* 数理情報学演習II H23_11_22_211.c */
/* マンデルブロ 変化量 */
/* 2011.11.28, 10RU004, 石橋祥太 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TMAX 100
#define XSIZE 900
#define YSIZE 900

void main(void)
{
	int countX, countY, countZ, countT, flag;
	double x, y, xt[TMAX + 1], yt[TMAX + 1], zt[TMAX], XMIN = -2.4, XMAX = 2.4, YMIN = -2.4, YMAX = 2.4,
		A = 0.2, B = -0.52, EP = 2.0, sT, EP2 = EP * EP;

	FILE *fp;

	fp = fopen("output.pgm", "w");
	fprintf(fp, "P2\n# 10RU004 石橋祥太\n%d %d\n255\n", XSIZE, YSIZE);

	for(countY = 0; countY < YSIZE; countY++)
	{
		y = (YMAX - YMIN) * countY / YSIZE + YMIN;
		for(countX = 0; countX < XSIZE; countX++)
		{
			x = (XMAX - XMIN) * countX / XSIZE + XMIN;

			yt[0] = xt[0] = 0.0;
			A = x;
			B = y;
			flag = 255;

			for(countZ = 0; countZ < TMAX; countZ++)
			{
				xt[countZ + 1] = xt[countZ] * xt[countZ] - yt[countZ] * yt[countZ] + A;
				yt[countZ + 1] = xt[countZ] * yt[countZ] * 2 + B;
				zt[countZ] = xt[countZ + 1] * xt[countZ + 1] + yt[countZ + 1] * yt[countZ + 1];
				if (zt[countZ] > EP2)
				{
					flag = (int)(abs(sqrt(zt[countZ]) - sqrt(zt[countZ - 1])) / EP2 * 255);
					break;
				}
			}
			fprintf(fp, "%d ", flag);
		}
	}
	fclose(fp);

	return;
}
