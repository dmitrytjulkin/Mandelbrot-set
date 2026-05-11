#pragma once

const int COLS_NUM     = 500;
const int ROWS_NUM     = 500;

const int RAND_COEF  = 4;
const int RED_COEF   = -3;
const int RED_MOD    = 256;
const int GREEN_COEF = 1;
const int GREEN_MOD  = 1;
const int BLUE_COEF  = -23;
const int BLUE_MOD   = 256;

void SetPixelColor (sf::VertexArray* pixels,
                    float offs_re, float offs_im, float scale);
