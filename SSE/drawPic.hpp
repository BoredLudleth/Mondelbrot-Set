#ifndef DRAWPIC_HPP
#define DRAWPIC_HPP

#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <x86intrin.h>
#include <immintrin.h>

#define DUP8(a) a, a, a, a, a, a, a, a

void countPix (unsigned char* pixels, float x0, float y0, float length);

void updateFrame (sf::Texture* mondelbrot, unsigned char* pixels, float x0, float y0, float length);

int mask_count (__m256 _mask);

#endif
