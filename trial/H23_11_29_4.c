#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

int main(void)
{
	int x, y, *mapsA, *mapsB, x1, y1;
	FILE *fp1;
	fp1 = fopen("se2_02_k4.pgm", "w");
	mapsA = (int *)malloc(sizeof(int) * 50 * 50);
	mapsB = (int *)malloc(sizeof(int) * 50 * 50);

	// 画像A生成
	for(x = 0; x < 50 * 50; x++) *(mapsA + x) = 200;

	*(mapsA + 40 + 5 * 50)
		= *(mapsA + 40 + 6 * 50)
		= *(mapsA + 40 + 7 * 50)
		= *(mapsA + 41 + 5 * 50)
		= *(mapsA + 41 + 6 * 50)
		= *(mapsA + 41 + 7 * 50)
		= 50;

	// 画像B生成(画像A回転)
	for(x = 0; x < 50 * 50; x++) *(mapsB + x) = 200;	//画像Bリセット
	for(y = 0; y < 50; y++)
		for(x = 0; x < 50; x++)
		{
			x1 = (int)(cos(20 * M_PI / 180) * x - sin(20 * M_PI / 180) * y + 0.5);
			y1 = (int)(sin(20 * M_PI / 180) * x + cos(20 * M_PI / 180) * y + 0.5);
			if (x1 >= 0 && x1 < 50 && y1 >= 0 && y1 < 50)
				*(mapsB + x1 + y1 * 50) = *(mapsA + x + y * 50);
		}


	// PGMファイル書き込み開始
	fprintf(fp1, "P2\n# 10RU004 石橋祥太\n50 50\n255\n");

	for(y = 0; y < 50; y++)
	{
		for(x = 0; x < 50; x++) fprintf(fp1, "%3d ", *(mapsB + x + y * 50));
		fprintf(fp1, "\n");
	}

	free(mapsB);
	free(mapsA);
	fclose(fp1);
	exit(0);
}
