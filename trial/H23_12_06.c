/* �������w���KII H23_12_06.c */
/* �}���f���u�� �W���΍�+�F����]+�o�̓��j���[ */
/* 2011.11.28, 10RU004, �΋��ˑ� */

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
	char latch, mjmode, colmode, file[256] = "output.ppm";
	int countX, countY, countZ, countT;
	double x, y, xt[TMAX + 1], yt[TMAX + 1], zt[TMAX], XMIN = -1.6-0.53, XMAX = 1.6-0.53, YMIN = -1.6, YMAX = 1.6,
		A = 0.2, B = -0.52, EP = 2.0, sT, fT, flag, ZMIN = DBL_MAX, ZMAX = DBL_MIN, inf = -log(0);

	FILE *fp;

	printf("�}�`�I��\n1:\t�}���f���u���W��\n2:\t�W�����A�W��\nOther:\t�I��\n>> ");
	scanf("%d", &mjmode);
	if (mjmode < 1 || mjmode > 2) return;

	printf("�F�ʃ��[�h�I��\n1:\t�ω���\n2:\t�W���΍�\nOther:\t�I��\n>> ");
	scanf("%d", &colmode);
	if (colmode < 1 || colmode > 2) return;

	printf("�`��͈͂����(X��)\n��) -2.13,1.07\n>> ");
	scanf("%lf,%lf", &XMIN, &XMAX);
	if (XMIN > XMAX) return;

	printf("�`��͈͂����(Y��)\n��) -1.6,1.6\n>> ");
	scanf("%lf,%lf", &YMIN, &YMAX);
	if (YMIN > YMAX) return;

	/*if ((XMAX - XMIN) * YSIZE > XSIZE * (YMAX - YMIN)) 
	else 
	*/

	for(countY = 0; countY < 256; file[countY++] = '\0');
	printf("�o�̓t�@�C���������(.ppm�������)\n>> ");
	scanf("%255s%*[^\n]", file);
	file[255] = '\0';	/* ���SNull */

	for(countY = 0; countY < 252; countY++)
		if (file[countY] == '\0')
		{
			if (countY < 5 || file[countY - 3] != '.'
				|| (file[countY - 3] != 'p' && file[countY - 3] != 'P')
				|| (file[countY - 2] != 'p' && file[countY - 2] != 'P')
				|| (file[countY - 1] != 'm' && file[countY - 1] != 'M'))
			{
				file[countY    ] = '.';
				file[countY + 1] = 
				file[countY + 2] = 'p';
				file[countY + 3] = 'm';
				file[countY + 4] = '\0';
			}
			break;
		}

	if ((fp = fopen(file, "w")) == NULL) exit(0);
	fprintf(fp, "P3\n# 10RU004 �΋��ˑ�\n%d %d\n255\n", XSIZE, YSIZE);

	for(latch = 0; latch < 2; latch++)
	{
		for(countY = 0; countY < YSIZE; countY++)
		{
			y = (YMAX - YMIN) * countY / YSIZE + YMIN;

			for(countX = 0; countX < XSIZE; countX++)
			{
				x = (XMAX - XMIN) * countX / XSIZE + XMIN;

				switch(mjmode)
				{
				case 1:
					xt[0] = yt[0] = 0.0;
					A = x;
					B = y;
					break;

				case 2:
					xt[0] = x;
					yt[0] = y;
					break;

				default:
					break;
				}

				flag = inf;

				for(countZ = 0; countZ < TMAX; countZ++)
				{
					xt[countZ + 1] = xt[countZ] * xt[countZ] - yt[countZ] * yt[countZ] + A;
					yt[countZ + 1] = xt[countZ] * yt[countZ] * 2 + B;
					zt[countZ] = sqrt(xt[countZ + 1] * xt[countZ + 1] + yt[countZ + 1] * yt[countZ + 1]);
					if (zt[countZ] > EP)
					{
						switch(colmode)
						{
							case 1:
								flag = zt[countZ] - zt[countZ - 1];
								break;

							case 2:
								countZ++;
								sT = fT = 0.0;
								for(countT = 0; countT < countZ; sT += zt[countT++]);
								sT /= countZ;
								for(countT = 0; countT < countZ; fT += (zt[countT] - sT) * (zt[countT++] - sT));
								flag = sqrt(fT / countZ);
								break;

							default:
								break;
						}
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
						flag = (M_PI * 2 * (flag - ZMIN) / (ZMAX - ZMIN));
						fprintf(fp, "%d %d %d ",
							(int)(255 * (cos(flag				) + 1) / 2),
							(int)(255 * (cos(flag - M_PI * 2 / 3) + 1) / 2),
							(int)(255 * (cos(flag - M_PI * 4 / 3) + 1) / 2));
						/* �F���� �ԉ��ΐ��� �S16777216�K�� */
					}
				}
			}
		}
	}
	fclose(fp);

	return;
}
