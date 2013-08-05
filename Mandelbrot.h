/* 数理情報学演習II Mandelbrot.h */
/* マンデルブロ 標準偏差+色相回転+画像マップ保持 */
/* 2012.01.16, 10RU004, 石橋祥太 */

#include <stdio.h>
#include <stdlib.h>

#ifndef Mandelbrot
	__declspec(dllexport) char Mandelbrot(const char *, unsigned long, double, double, double, double, double, double, double, double, unsigned short, unsigned short);
#endif
