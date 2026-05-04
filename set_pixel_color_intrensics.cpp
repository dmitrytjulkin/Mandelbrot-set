#include <SFML/Graphics.hpp>
#include <immintrin.h>

#include "mandelbrot.h"

const int OFFS_CENT_RE     = COLS_NUM / 2;
const int OFFS_CENT_IM     = ROWS_NUM / 2;
const int RADIUS_2         = 4;
const int MAX_ITER_VAL = 256;
const int BUF_SIZE         = 8;

void SetPixelColorBy4 (sf::VertexArray* pixels,
                       float offs_re, float offs_im, float scale)
{
    float multiplier_x = 4 / (float) COLS_NUM;
    float multiplier_y = 4 / (float) ROWS_NUM;

    float x_angle_coef = multiplier_x / scale;
    float y_angle_coef = multiplier_y / scale;
    float x_free_coef  = multiplier_x * (OFFS_CENT_RE / scale + offs_re);
    float y_free_coef  = multiplier_y * (OFFS_CENT_IM / scale + offs_im);

    __m256 x_angle_vec = {};
    __m256 y_angle_vec = {};
    __m256 x_free_vec  = {};
    __m256 y_free_vec  = {};

    x_angle_vec = _mm256_set1_ps (x_angle_coef);
    y_angle_vec = _mm256_set1_ps (y_angle_coef);
    x_free_vec = _mm256_set1_ps (x_free_coef);
    y_free_vec = _mm256_set1_ps (y_free_coef);

    __m256i xi_vec = {};
    __m256i yi_vec = {};
    __m256i i_vec  = {};

    __m256 xf_vec = {};
    __m256 yf_vec = {};

    __m256 re0_vec = {};
    __m256 im0_vec = {};

    __m256 ren_vec  = {};
    __m256 imn_vec  = {};
    __m256 re2n_vec = {};
    __m256 im2n_vec = {};

    __m256i  cmprbl_vec  = {};
    __m256i cmp_res_vec = {};

    __m256i n_vec      = {};
    __m256i mode_n_vec = {};

    for (int x = 0; x < COLS_NUM; ++x) {
        for (int y = 0; y < ROWS_NUM; y += BUF_SIZE) {
            xi_vec = _mm256_set1_epi32 (x);

            yi_vec = _mm256_set1_epi32 (y);
            i_vec = _mm256_setr_epi32 (0, 1, 2, 3, 4, 5, 6, 7);
            yi_vec = _mm256_add_epi32 (yi_vec, i_vec);

            xf_vec = _mm256_set1_ps (x);
            yf_vec = _mm256_set1_ps (y);

            re0_vec = _mm256_fmsub_ps (xf_vec, x_angle_vec, x_free_vec);
            im0_vec = _mm256_fmsub_ps (yf_vec, y_angle_vec, y_free_vec);

            ren_vec  = _mm256_setzero_ps ();
            imn_vec  = _mm256_setzero_ps ();
            re2n_vec = _mm256_setzero_ps ();
            im2n_vec = _mm256_setzero_ps ();

            cmprbl_vec  = _mm256_setzero_si256 ();
            cmp_res_vec = _mm256_setzero_si256 ();

            n_vec      = _mm256_setzero_si256 ();
            mode_n_vec = _mm256_set1_epi32 (1);

            __m256i pixel_coords_vec = {};
            int x_arr[BUF_SIZE] = {};
            int y_arr[BUF_SIZE] = {};
            int n_arr[BUF_SIZE] = {};

            int iteration = 0;
            while (iteration < MAX_ITER_VAL){
                __m256 temp_vec = ren_vec;
                temp_vec    = _mm256_mul_ps (_mm256_set1_ps (2), ren_vec);                 // temp_vec = 2 * ren_vec

                re2n_vec    = _mm256_mul_ps (ren_vec, ren_vec);
                im2n_vec    = _mm256_mul_ps (imn_vec, imn_vec);
                ren_vec     = _mm256_sub_ps (re2n_vec, im2n_vec);
                ren_vec     = _mm256_add_ps (ren_vec, re0_vec);                 // ren_vec = re2n_vec + im2n_vec + re0_vec

                imn_vec     = _mm256_fmsub_ps (temp_vec, imn_vec, im0_vec);     // imn_vec = temp_vec * imn_vec + im0_vec

                cmprbl_vec  = _mm256_castps_si256 (_mm256_add_ps (re2n_vec, im2n_vec));
                cmp_res_vec = _mm256_cmpgt_epi32 (_mm256_set1_epi32 (RADIUS_2), cmprbl_vec);        // RADIUS_2 > re2n + im2n
                cmp_res_vec = _mm256_and_si256 (cmp_res_vec, _mm256_set1_epi32 (1));
                mode_n_vec  = _mm256_mul_epi32 (cmp_res_vec, mode_n_vec);       // mode_n_vec[i] = 1 if true, 0 otherwise
                n_vec       = _mm256_add_epi32 (n_vec, mode_n_vec);             // n_i += mode_n_vec[i]

                ++iteration;
            }

            _mm256_store_si256 ((__m256i*) x_arr, xi_vec);
            _mm256_store_si256 ((__m256i*) y_arr, yi_vec);
            _mm256_store_si256 ((__m256i*) n_arr, n_vec);

            for (int i = 0; i < BUF_SIZE; i++) {
                sf::Color color = sf::Color::Black;

                if (n_arr[i] < MAX_ITER_VAL)
                    sf::Color color = sf::Color ((-n_arr[i] * 3) % 256, n_arr[i], (-n_arr[i] * 23) % 256);

                (*pixels)[x_arr[i] * COLS_NUM + y_arr[i]] =
                            sf::Vertex (sf::Vector2f (x_arr[i], y_arr[i]), color);
            }
        }
    }
}
