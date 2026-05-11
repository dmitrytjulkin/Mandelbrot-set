#include <SFML/Graphics.hpp>

#include "mandelbrot.h"

const int SCALE_MULTIPLIER = 2;
const int VIEW_OFFSET_VAL  = 25;

const int CHAR_SIZE = 36;
const float TEXT_POS_X = 5;
const float TEXT_POS_Y = 5;
const int LONGEST_FPS_PHRASE = 21;

#define IS_PRESSED(key) sf::Keyboard::isKeyPressed(sf::Keyboard::key)

void TransformView  (float* x_coord, float* y_coord, int step,
                     float* scale, bool* need_update);
void SetFpsPhrase   (sf::Text* text, float delta_time);
void PrintWindow    (sf::RenderWindow* window, sf::VertexArray pixels,
                     sf::Text text);

int main ()
{
    sf::RenderWindow window (sf::VideoMode (COLS_NUM, ROWS_NUM), "Mandelbrot");
    window.setFramerateLimit(0);

    sf::VertexArray pixels (sf::Points, COLS_NUM * ROWS_NUM);
    sf::Event event = {};
    sf::Clock clock = {};

    sf::Text text = {};
    sf::Font arial_font = {};
    if (!arial_font.loadFromFile("/home/tyulkin_dmitriy/TDA projects/Mandelbrot-set/arial.ttf"))
       printf("ERROR LOADING FONT\n");
    text.setFont (arial_font);
    text.setCharacterSize (CHAR_SIZE);
    text.setPosition (TEXT_POS_X, TEXT_POS_Y);
    text.setFillColor(sf::Color::White);

    float offs_re = 0, offs_im = 0;
    float scale       = 1;
    float delta_time  = 0;
    float fps         = 0;
    bool  need_update = true;

    while (window.isOpen ())
    {
        TransformView (&offs_re, &offs_im, VIEW_OFFSET_VAL,
                       &scale, &need_update);

        clock.restart();
        SetPixelColor (&pixels, offs_re, offs_im, scale);
        delta_time = clock.restart().asSeconds();

        SetFpsPhrase (&text, delta_time);

        if (!need_update) continue;
        need_update = false;

        printf ("the x offset %0.6lg\n", offs_re);
        printf ("the y offset %0.6lg\n", offs_im);
        printf ("the scale    %0.6lg\n", scale);

        float time = 0;
        PrintWindow (&window, pixels, text);
        time = clock.restart().asSeconds();
        printf ("the time consumed = %f\n", time);
    }

    return 0;
}

void TransformView (float* x_coord, float* y_coord, int step,
                    float* scale, bool* need_update)
{
    if (IS_PRESSED(A)) {*x_coord += step / *scale; *need_update = true;}
    if (IS_PRESSED(D)) {*x_coord -= step / *scale; *need_update = true;}
    if (IS_PRESSED(S)) {*y_coord -= step / *scale; *need_update = true;}
    if (IS_PRESSED(W)) {*y_coord += step / *scale; *need_update = true;}

    if (IS_PRESSED(Equal)) {
        *scale      *= SCALE_MULTIPLIER;
        *need_update = true;
    }

    if (IS_PRESSED(Dash)) {
        *scale      /= SCALE_MULTIPLIER;
        *need_update = true;
    }
}

void SetFpsPhrase (sf::Text* text, float delta_time)
{
    float fps = 1 / delta_time;
    printf("fps = %lg\n", fps);

    char fps_phrase[LONGEST_FPS_PHRASE] = {};
    sprintf (fps_phrase, "FPS = %.2lg", fps);

    text->setString (fps_phrase);
}

void PrintWindow (sf::RenderWindow* window, sf::VertexArray pixels,
                  sf::Text text)
{
    float time = 0;
    printf("window draw start\n");

    window->clear ();
    window->draw (pixels);
    window->draw (text);
    window->display ();

    printf("window draw end with consumed time = %f\n", time);
}
