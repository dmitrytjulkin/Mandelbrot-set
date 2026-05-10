#include <SFML/Graphics.hpp>

#include "mandelbrot.h"

const int OFFS_CENT_RE     = COLS_NUM / 2;
const int OFFS_CENT_IM     = ROWS_NUM / 2;
const int RADIUS           = 2;
const int NUM_TO_STOP_CALC = 256;

void SetPixelColorBuffered (sf::VertexArray* pixels, const int buf_size,
                       float offs_re, float offs_im, float scale)
{
    float multiplier_x = RAND_COEF / (float) COLS_NUM;
    float multiplier_y = RAND_COEF / (float) ROWS_NUM;

    float x_angle_coef = multiplier_x / scale;
    float y_angle_coef = multiplier_y / scale;
    float x_free_coef  = multiplier_x * (OFFS_CENT_RE / scale + offs_re);
    float y_free_coef  = multiplier_y * (OFFS_CENT_IM / scale + offs_im);

    float x_angle_arr[buf_size] = {};
    float y_angle_arr[buf_size] = {};
    float x_free_arr[buf_size]  = {};
    float y_free_arr[buf_size]  = {};

    for (int k = 0; k < buf_size; ++k) x_angle_arr[k] = x_angle_coef;
    for (int k = 0; k < buf_size; ++k) y_angle_arr[k] = y_angle_coef;
    for (int k = 0; k < buf_size; ++k) x_free_arr[k]  = x_free_coef;
    for (int k = 0; k < buf_size; ++k) y_free_arr[k]  = y_free_coef;

    for (int x = 0; x < COLS_NUM; ++x) {
        for (int y = 0; y < ROWS_NUM; y += buf_size) {
            int x_arr[buf_size] = {};
            int y_arr[buf_size] = {};

            for (int i = 0; i < buf_size; ++i)  x_arr[i] = x;
            for (int i = 0; i < buf_size; ++i)  y_arr[i] = y + i;

            float re0_arr[buf_size] = {};
            float im0_arr[buf_size] = {};
            for (int i = 0; i < buf_size; ++i){
                re0_arr[i] = x_arr[i] * x_angle_arr[i] - x_free_arr[i];
                im0_arr[i] = y_arr[i] * y_angle_arr[i] - y_free_arr[i];
            }

            float ren_arr[buf_size] = {};
            float imn_arr[buf_size] = {};
            float re2n_arr[buf_size] = {};
            float im2n_arr[buf_size] = {};

            int n[buf_size] = {};
            for (int i = 0; i < buf_size; ++i) {
                do {
                    float tmp = ren_arr[i];
                    ren_arr[i] = re2n_arr[i] - im2n_arr[i] + re0_arr[i];
                    imn_arr[i] = 2 * tmp * imn_arr[i] + im0_arr[i];

                    for (int j = 0; j < buf_size; ++j) re2n_arr[j] = ren_arr[j] * ren_arr[j];
                    for (int j = 0; j < buf_size; ++j) im2n_arr[j] = imn_arr[j] * imn_arr[j];

                    ++n[i];
                } while (RADIUS * RADIUS >= re2n_arr[i] + im2n_arr[i] && n[i] < NUM_TO_STOP_CALC);
            }

            for (int i = 0; i < buf_size; i++) {
                if (n[i] >= NUM_TO_STOP_CALC) {
                    sf::Color color = sf::Color::Black;
                    (*pixels)[x_arr[i] * COLS_NUM + y_arr[i]] =
                            sf::Vertex (sf::Vector2f (x_arr[i], y_arr[i]), color);

                    continue;
                }

                color = sf::Color ((n[i] * RED_COEF) % RED_MOD,
                                   (n[i] * GREEN_COEF) % GREEN_MOD,
                                   (n[i] * BLUE_COEF) % BLUE_MOD);
                (*pixels)[x_arr[i] * COLS_NUM + y_arr[i]] =
                        sf::Vertex (sf::Vector2f (x_arr[i], y_arr[i]), color);
            }
        }
    }
}
