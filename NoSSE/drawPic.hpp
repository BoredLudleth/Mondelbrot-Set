#ifndef DRAWPIC_HPP
#define DRAWPIC_HPP

#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void countPix (unsigned char* pixels, float x0, float y0, float length);

void updateFrame (sf::Texture* mondelbrot, unsigned char* pixels, float x0, float y0, float length);

#endif
