#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int x, y,
		img[3][5] = {
			{255, 255, 255, 255, 255},
			{255,   0, 255,   0, 255},
			{255, 255, 255, 255, 255}
	};
	FILE *fp1;
	fp1 = fopen("se2_02_k1.pgm", "w");

	fprintf(fp1, "P2\n# 10RU004 Î‹´Ë‘¾\n3 5\n255\n");

	for(y = 0; y < 3; y++)
	{
		for(x = 0; x < 5; x++) fprintf(fp1, "%3d ", img[y][x]);
		fprintf(fp1, "\n");
	}

	fclose(fp1);
	exit(0);
}
