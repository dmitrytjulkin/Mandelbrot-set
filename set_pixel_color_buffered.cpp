#include <SFML/Graphics.hpp>

#include "mandelbrot.h"

const int OFFS_CENT_RE     = COLS_NUM / 2;
const int OFFS_CENT_IM     = ROWS_NUM / 2;
const int RADIUS           = 2;
const int NUM_TO_STOP_CALC = 256;

const int BUF_SIZE = 4;

void FindMandelbrotNumber (int* n_arr, float* re0_arr, float* im0_arr);
void ColorPixels (sf::VertexArray* pixels,
                  int* x_arr, int* y_arr, int* n_arr);

void SetPixelColor (sf::VertexArray* pixels,
                       float offs_re, float offs_im, float scale)
{
    float multiplier_x = RAND_COEF / (float) COLS_NUM;
    float multiplier_y = RAND_COEF / (float) ROWS_NUM;

    float x_angle_coef = multiplier_x / scale;
    float y_angle_coef = multiplier_y / scale;
    float x_free_coef  = multiplier_x * (OFFS_CENT_RE / scale + offs_re);
    float y_free_coef  = multiplier_y * (OFFS_CENT_IM / scale + offs_im);

    float x_angle_arr[BUF_SIZE] = {};
    float y_angle_arr[BUF_SIZE] = {};
    float x_free_arr[BUF_SIZE]  = {};
    float y_free_arr[BUF_SIZE]  = {};

    for (int k = 0; k < BUF_SIZE; ++k) x_angle_arr[k] = x_angle_coef;
    for (int k = 0; k < BUF_SIZE; ++k) y_angle_arr[k] = y_angle_coef;
    for (int k = 0; k < BUF_SIZE; ++k) x_free_arr[k]  = x_free_coef;
    for (int k = 0; k < BUF_SIZE; ++k) y_free_arr[k]  = y_free_coef;

    for (int x = 0; x < COLS_NUM; ++x) {
        for (int y = 0; y < ROWS_NUM; y += BUF_SIZE) {
            int x_arr[BUF_SIZE] = {};
            int y_arr[BUF_SIZE] = {};
            for (int i = 0; i < BUF_SIZE; ++i)  x_arr[i] = x;
            for (int i = 0; i < BUF_SIZE; ++i)  y_arr[i] = y + i;

            float re0_arr[BUF_SIZE] = {};
            float im0_arr[BUF_SIZE] = {};
            for (int i = 0; i < BUF_SIZE; ++i)
                re0_arr[i] = x_arr[i] * x_angle_arr[i] - x_free_arr[i];
            for (int i = 0; i < BUF_SIZE; ++i)
                im0_arr[i] = y_arr[i] * y_angle_arr[i] - y_free_arr[i];

            int n_arr[BUF_SIZE] = {};
            FindMandelbrotNumber (n_arr, re0_arr, im0_arr);

            ColorPixels (pixels, x_arr, y_arr, n_arr);
        }
    }
}

void FindMandelbrotNumber (int* n_arr, float* re0_arr, float* im0_arr)
{
    float ren_arr[BUF_SIZE]  = {};
    float imn_arr[BUF_SIZE]  = {};
    float re2n_arr[BUF_SIZE] = {};
    float im2n_arr[BUF_SIZE] = {};

    for (int i = 0; i < BUF_SIZE; ++i) {
        do {
            float tmp = 2 * ren_arr[i];
            ren_arr[i] = re2n_arr[i] - im2n_arr[i] + re0_arr[i];
            imn_arr[i] = tmp * imn_arr[i] + im0_arr[i];

            for (int j = 0; j < BUF_SIZE; ++j) re2n_arr[j] = ren_arr[j] * ren_arr[j];
            for (int j = 0; j < BUF_SIZE; ++j) im2n_arr[j] = imn_arr[j] * imn_arr[j];

            ++n_arr[i];
        } while (RADIUS * RADIUS >= re2n_arr[i] + im2n_arr[i] && n_arr[i] < NUM_TO_STOP_CALC);
    }
}

void ColorPixels (sf::VertexArray* pixels, int* x_arr, int* y_arr, int* n_arr)
{
    for (int i = 0; i < BUF_SIZE; i++) {
        sf::Color color = {};

        if (n_arr[i] >= NUM_TO_STOP_CALC)
            color = sf::Color::Black;

        else
            color = sf::Color ((n_arr[i] * RED_COEF) % RED_MOD,
                            (n_arr[i] * GREEN_COEF) % GREEN_MOD,
                            (n_arr[i] * BLUE_COEF) % BLUE_MOD);

        (*pixels)[x_arr[i] * COLS_NUM + y_arr[i]] =
                sf::Vertex (sf::Vector2f (x_arr[i], y_arr[i]), color);
    }
}
