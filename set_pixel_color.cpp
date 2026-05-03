#include <SFML/Graphics.hpp>

#include "mandelbrot.h"

const int OFFS_CENT_RE = COLS_NUM / 2;
const int OFFS_CENT_IM = ROWS_NUM / 2;
const int RADIUS       = 2;
const int NUM_TO_STOP_CALC = 256;

void SetPixelColor (sf::VertexArray* pixels,
                     float offs_re, float offs_im, float scale)
{
    float multiplier_x = 4 / (float) COLS_NUM;
    float multiplier_y = 4 / (float) ROWS_NUM;

    float x_angle_coef = multiplier_x / scale;
    float y_angle_coef = multiplier_y / scale;
    float x_free_coef  = multiplier_x * (OFFS_CENT_RE / scale + offs_re);
    float y_free_coef  = multiplier_y * (OFFS_CENT_IM / scale + offs_im);

    for (int x = 0; x < COLS_NUM; ++x) {
        for (int y = 0; y < ROWS_NUM; ++y) {
            float re_0 = x_angle_coef * x - x_free_coef;
            float im_0 = y_angle_coef * y - y_free_coef;
            float re_n = 0, im_n = 0;

            int n = 0;
            while (RADIUS * RADIUS >= re_n * re_n + im_n * im_n && n < NUM_TO_STOP_CALC) {
                float re_2n = re_n * re_n, im_2n = im_n * im_n;
                float tmp = re_n;
                re_n = re_2n - im_2n + re_0;
                im_n = 2 * tmp * im_n + im_0;

                ++n;
            }

            if (n >= NUM_TO_STOP_CALC) {
                sf::Color color = sf::Color::Black;
                (*pixels)[x * COLS_NUM + y] = sf::Vertex (sf::Vector2f (x, y), color);

                continue;
            }

            sf::Color color = sf::Color ((-n * 3) % 256, n, (-n * 23) % 256);
            (*pixels)[x * COLS_NUM + y] = sf::Vertex (sf::Vector2f (x, y), color);
        }
    }
}
