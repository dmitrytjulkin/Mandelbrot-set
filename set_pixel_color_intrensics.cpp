#include <SFML/Graphics.hpp>
#include <immintrin.h>

#include "mandelbrot.h"

const int OFFS_CENT_RE     = COLS_NUM / 2;
const int OFFS_CENT_IM     = ROWS_NUM / 2;
const int RADIUS_2         = 4;
const int MAX_ITER_VAL     = 256;
const int BUF_SIZE         = 8;


void ColorPixels (sf::VertexArray* pixels, __m256i x_vec, __m256i y_vec, __m256i n_vec);
__m256i FindMandelbrotNumber (__m256 re0_vec, __m256 im0_vec);

void SetPixelColorIntrensics (sf::VertexArray* pixels,
                       float offs_re, float offs_im, float scale)
{
    float multiplier_x = RAND_COEF / (float) COLS_NUM;
    float multiplier_y = RAND_COEF / (float) ROWS_NUM;

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
    x_free_vec  = _mm256_set1_ps (x_free_coef);
    y_free_vec  = _mm256_set1_ps (y_free_coef);

    __m256i x_vec = {};
    __m256i y_vec = {};
    __m256i i_vec = {};

    __m256 re0_vec = {};
    __m256 im0_vec = {};

    __m256i n_vec = {};

    for (int x = 0; x < COLS_NUM; ++x) {
        for (int y = 0; y < ROWS_NUM; y += BUF_SIZE) {
            x_vec = _mm256_set1_epi32 (x);
            y_vec = _mm256_set1_epi32 (y);
            i_vec = _mm256_setr_epi32 (0, 1, 2, 3, 4, 5, 6, 7);
            y_vec = _mm256_add_epi32 (y_vec, i_vec);

            re0_vec = _mm256_fmsub_ps (_mm256_cvtepi32_ps (x_vec), x_angle_vec, x_free_vec);
            im0_vec = _mm256_fmsub_ps (_mm256_cvtepi32_ps (y_vec), y_angle_vec, y_free_vec);

            n_vec = FindMandelbrotNumber (re0_vec, im0_vec);

            ColorPixels (pixels, x_vec, y_vec, n_vec);
        }
    }
}

__m256i FindMandelbrotNumber (__m256 re0_vec, __m256 im0_vec)
{
    __m256 ren_vec  = {};
    __m256 imn_vec  = {};
    __m256 re2n_vec = {};
    __m256 im2n_vec = {};

    ren_vec  = _mm256_setzero_ps ();
    imn_vec  = _mm256_setzero_ps ();
    re2n_vec = _mm256_setzero_ps ();
    im2n_vec = _mm256_setzero_ps ();

    __m256  cmprbl_vec      = {};
    __m256i cmp_res_vec     = {};
    __m256i is_pixel_in_vec = {};
    __m256i n_vec           = {};

    cmprbl_vec      = _mm256_setzero_ps ();
    cmp_res_vec     = _mm256_setzero_si256 ();
    n_vec           = _mm256_setzero_si256 ();
    is_pixel_in_vec = _mm256_set1_epi32 (1);

    int iteration = 0;
    while (iteration < MAX_ITER_VAL){
        __m256 temp_vec = _mm256_mul_ps (_mm256_set1_ps (2), ren_vec);

        re2n_vec    = _mm256_mul_ps (ren_vec, ren_vec);
        im2n_vec    = _mm256_mul_ps (imn_vec, imn_vec);
        ren_vec     = _mm256_add_ps (_mm256_sub_ps (re2n_vec, im2n_vec), re0_vec);
        imn_vec     = _mm256_fmadd_ps (temp_vec, imn_vec, im0_vec);

        cmprbl_vec      = _mm256_add_ps (re2n_vec, im2n_vec);
        cmp_res_vec     = _mm256_castps_si256 (_mm256_cmp_ps (_mm256_set1_ps (RADIUS_2), cmprbl_vec, _CMP_GT_OS));
        cmp_res_vec     = _mm256_srli_epi32 (cmp_res_vec, 31);
        is_pixel_in_vec = _mm256_and_si256 (cmp_res_vec, is_pixel_in_vec);
        n_vec           = _mm256_add_epi32 (n_vec, is_pixel_in_vec);

        ++iteration;
    }

    return n_vec;
}

void ColorPixels (sf::VertexArray* pixels, __m256i x_vec, __m256i y_vec, __m256i n_vec)
{
    int x_arr[BUF_SIZE] = {};
    int y_arr[BUF_SIZE] = {};
    int n_arr[BUF_SIZE] = {};

    _mm256_storeu_si256 ((__m256i*) x_arr, x_vec);
    _mm256_storeu_si256 ((__m256i*) y_arr, y_vec);
    _mm256_storeu_si256 ((__m256i*) n_arr, n_vec);

    for (int i = 0; i < BUF_SIZE; i++) {
        sf::Color color;

        if (n_arr[i] <= MAX_ITER_VAL)
            color = sf::Color ((n_arr[i] * RED_COEF) % RED_MOD,
                               (n_arr[i] * GREEN_COEF) % GREEN_MOD,
                               (n_arr[i] * BLUE_COEF) % BLUE_MOD);

        else
            color = sf::Color::Black;

        (*pixels)[x_arr[i] * COLS_NUM + y_arr[i]] =
                    sf::Vertex (sf::Vector2f (x_arr[i], y_arr[i]), color);
    }
}
