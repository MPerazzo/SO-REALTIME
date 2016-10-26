#ifndef FRACTALS_H_
#define FRACTALS_H_

#include <stdint.h>

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768
#define DEFAULT_BPP 24

void mandelbrot(int iter, uint8_t r, uint8_t g, uint8_t b, uint8_t r2, uint8_t g2, uint8_t b2);

void juliaSet();

#endif