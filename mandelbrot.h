#pragma once

const int COLS_NUM     = 500;
const int ROWS_NUM     = 500;

void SetPixelColor (sf::VertexArray* pixels,
                     float offs_re, float offs_im, float scale);

void SetPixelColorBy4 (sf::VertexArray* pixels,
                       float offs_re, float offs_im, float scale);

void SetPixelColorIntrensics (sf::VertexArray* pixels,
                              float offs_re, float offs_im, float scale);


