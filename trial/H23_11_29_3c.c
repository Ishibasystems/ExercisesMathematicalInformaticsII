#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int x, y;
	FILE *fp1;
	fp1 = fopen("se2_02_k3c.pgm", "w");

	fprintf(fp1, "P2\n# 10RU004 Î‹´Ë‘¾\n50 50\n255\n");

	for(y = 0; y < 50; y++)
	{
		for(x = 0; x < 50; x++)
			if ((x + y) % 2)
				fprintf(fp1, "255 ");
			else
				fprintf(fp1, "  0 ");

		fprintf(fp1, "\n");
	}

	fclose(fp1);
	exit(0);
}
