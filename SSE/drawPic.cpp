#include "drawPic.hpp"

void countPix (unsigned char* pixels, float x0, float y0, float length) {
    int counter = 0;
    int current_p = 0;

    float max_r = 10;

    float dx = length / 1000;

    const float cx0 = x0;

    __m256 _maxr = _mm256_set_ps (DUP8(max_r));

    __m256 _x0 = _mm256_set_ps  (x0, x0 + dx, x0 + 2 * dx, x0 + 3 * dx, x0 + 4 * dx, x0 + 5 * dx, x0 + 6 * dx, x0 + 7 * dx);
    __m256 _y0 = _mm256_set_ps  (DUP8(y0));
    __m256 _xn = _mm256_set_ps  (x0, x0 + dx, x0 + 2 * dx, x0 + 3 * dx, x0 + 4 * dx, x0 + 5 * dx, x0 + 6 * dx, x0 + 7 * dx);
    __m256 _yn = _mm256_set_ps  (DUP8(y0));

    __m256 _x2 = _mm256_mul_ps (_xn, _xn);
    __m256 _y2 = _mm256_mul_ps (_yn, _yn);
    __m256 _xy = _mm256_mul_ps (_xn, _yn);

    __m256 _counter = _mm256_setzero_ps ();

    __m256 _increase = _mm256_set_ps (DUP8(1));

    __m256 _rcur = _mm256_setzero_ps ();

    __m256 _mask = _mm256_cmp_ps (_rcur, _maxr, _CMP_LE_OS);
    
    for (int pix_y = 0; pix_y < 1000; pix_y++) {
        y0 -= dx;
        x0 = cx0;
        for (int pix_x = 0; pix_x < 1000; pix_x += 8) {
            _x0 = _mm256_set_ps  (x0, x0 + dx, x0 + 2 * dx, x0 + 3 * dx, x0 + 4 * dx, x0 + 5 * dx, x0 + 6 * dx, x0 + 7 * dx);
            _y0 = _mm256_set_ps  (DUP8(y0));

            _xn = _mm256_set_ps  (x0, x0 + dx, x0 + 2 * dx, x0 + 3 * dx, x0 + 4 * dx, x0 + 5 * dx, x0 + 6 * dx, x0 + 7 * dx);
            _yn = _mm256_set_ps  (DUP8(y0));

            _counter = _mm256_setzero_ps ();

            _mask = _mm256_cmp_ps (_increase, _maxr, _CMP_LE_OS);

            counter = 0;
            
            while (counter < 255 && (mask_count (_mask) != 0)) {
                _x2 = _mm256_mul_ps (_xn, _xn);
                _y2 = _mm256_mul_ps (_yn, _yn);
                _xy = _mm256_mul_ps (_xn, _yn);

                _xn = _mm256_sub_ps (_x2, _y2);
                _xn = _mm256_add_ps (_xn, _x0);

                _yn = _mm256_add_ps (_xy, _xy);
                _yn = _mm256_add_ps (_yn, _y0);

                _rcur = _mm256_add_ps (_mm256_mul_ps (_xn, _xn), _mm256_mul_ps (_yn, _yn));

                _mask = _mm256_cmp_ps (_rcur, _maxr, _CMP_LE_OS);

                _counter = _mm256_add_ps (_counter, _mm256_and_ps (_mask, _increase));

                counter++;
            }

            current_p += 32;
            x0 += 8 * dx;

            float* count = (float*) &_counter;

            for (int i = 0; i < 8; i++) {
                pixels[current_p  - i * 4] = 255 - (int) count[i];
                pixels[current_p  - i * 4 + 1] = (((int) count[i])%4) * 64;
                pixels[current_p  - i * 4  + 2] = 128;
                pixels[current_p  - i * 4 + 3] = 255 - (int) count[i];
            }
        }   
    }
}

void updateFrame (sf::Texture* mondelbrot, unsigned char* pixels, float x0, float y0, float length) {
    countPix (pixels, x0, y0, length);
    (*mondelbrot).update(pixels, 1000, 1000, 0, 0);
}

int mask_count (__m256 _mask) {
    __m256 _checker = _mm256_set_ps (DUP8(1));
    _checker = _mm256_and_ps (_mask, _checker);

    float* check = (float*) &_checker;
    // printf("%d", check[0]);

    float result = 0;
    for (int i = 0; i < 8; i++) {
        result += check[i];
        //printf ("result: %d", result);
    }

    return (int) result;
}