#include "drawPic.hpp"

void countPix (unsigned char* pixels, float x0, float y0, float length) {
    int counter = 0;
    int current_p = 0;

    float x_2 = 0;
    float y_2 = 0;
    float xy = 0;

    float max_r = 10;

    float dx = length / 1000;
//TODO: somecheck, if smth go wrong
    const float cx0 = x0;

    for (int pix_y = 0; pix_y < 1000; pix_y++) {
        y0 -= dx;
        x0 = cx0;
        for (int pix_x = 0; pix_x < 1000; pix_x++, current_p += 4) {
            x0 += dx;

            for (float x_n = x0, y_n = y0; counter < 255; counter++) {
                x_2 = x_n * x_n;
                y_2 = y_n * y_n;
                xy = x_n * y_n;

                if ((x_2 + y_2) >= max_r) {
                    break;
                } else {
                    x_n = x_2 - y_2 + x0; 
                    y_n = xy + xy + y0; 
                }
            }

            pixels[current_p] = 255 - counter;
            pixels[current_p + 1] = (counter%4) * 64;
            pixels[current_p + 2] = 128;
            pixels[current_p + 3] = 255 - counter;
            counter = 0;
        }
    }
}

void updateFrame (sf::Texture* mondelbrot, unsigned char* pixels, float x0, float y0, float length) {
    countPix (pixels, x0, y0, length);
    (*mondelbrot).update(pixels,1000,1000,0,0);
}