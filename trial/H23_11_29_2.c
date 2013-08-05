#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int x, y;
	FILE *fp1;
	fp1 = fopen("se2_02_k2.pgm", "w");

	fprintf(fp1, "P2\n# 10RU004 Î‹´Ë‘¾\n41 41\n255\n");

	for(y = 0; y < 41; y++)
	{
		for(x = 0; x < 41; x++)
			if (x == 20)
				fprintf(fp1, "200 ");
			else
				fprintf(fp1, "128 ");
		fprintf(fp1, "\n");
	}

	fclose(fp1);
	exit(0);
}
