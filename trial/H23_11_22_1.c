#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TMAX 100
#define XSIZE 300
#define YSIZE 300

void main(void)
{
	int countX, countY, countZ, flag;
	double x, y, xt[TMAX + 1], yt[TMAX + 1], XMIN = -1.2, XMAX = 1.2, YMIN = -1.2, YMAX = 1.2;
	double A = 0.2, B = -0.52, EP = 2.0;

	FILE *fp;

	fp = fopen("test.pgm", "w");

	fprintf(fp, "P2\n");
	fprintf(fp, "# 10RU004 êŒã¥èÀëæ\n");
	fprintf(fp, "300 300\n");
	fprintf(fp, "1\n");

	for(countY = 0; countY < YSIZE; countY++)
	{
		y = (YMAX - YMIN) * countY / YSIZE + YMIN;
		for(countX = 0; countX < XSIZE; countX++)
		{
			x = (XMAX - XMIN) * countX / XSIZE + XMIN;

			xt[0] = 0.0;
			A = x;
			yt[0] = 0.0;
			B = y;
			flag = 1;

			for(countZ = 0; countZ < TMAX; countZ++)
			{
				xt[countZ + 1] = xt[countZ] * xt[countZ] - yt[countZ] * yt[countZ] + A;
				yt[countZ + 1] = xt[countZ] * yt[countZ] * 2 + B;
				if (sqrt(xt[countZ + 1] * xt[countZ + 1] + yt[countZ + 1] * yt[countZ + 1]) > EP)
				{
					flag = 0;
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