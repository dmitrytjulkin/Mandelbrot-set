#include <SFML/Graphics.hpp>
#include <vector>
#include <unistd.h>

const int COLS_NUM     = 500;
const int ROWS_NUM     = 500;
const int OFFS_CENT_RE = COLS_NUM / 2;
const int OFFS_CENT_IM = ROWS_NUM / 2;
const int RADIUS       = 2;

const int SCALE_MULTIPLIER = 2;
const int VIEW_OFFSET_VAL  = 50;

const int NUM_TO_STOP_CALC = 256;

#define IS_PRESSED(key) sf::Keyboard::isKeyPressed(sf::Keyboard::key)

void TransformView  (float* x_coord, float* y_coord, int step,
                     float* scale, bool* need_update);
void SetPixelsColor (sf::VertexArray* pixels,
                     float offs_re, float offs_im, float scale);

int main ()
{
    sf::RenderWindow window (sf::VideoMode (COLS_NUM, ROWS_NUM), "Mandelbrot");
    printf("Window created\n");
    window.setFramerateLimit(0);

    sf::VertexArray pixels (sf::Points, COLS_NUM * ROWS_NUM);
    sf::Event event = {};
    sf::Clock clock = {};
    sf::Text text = {};
    sf::Font arial_font = {};
    arial_font.loadFromFile ("/root/TDA projects/Mandelbrot-set/arial.ttf");
    printf("Font loaded\n");

    float offs_re = 0, offs_im = 0;
    float scale = 1;
    float delta_time = 0;
    float fps = 0;
    bool need_update = true;

    while (window.isOpen ())
    {
        SetPixelsColor (&pixels, offs_re, offs_im, scale);

        TransformView (&offs_re, &offs_im, VIEW_OFFSET_VAL,
                       &scale, &need_update);

        delta_time = clock.restart().asSeconds();
        fps = 1 / delta_time;
        printf("fps = %.2lg\n", fps);

        char fps_phrase[21] = {};
        sprintf (fps_phrase, "FPS = %.2lg", fps);
        text.setFont (arial_font);
        text.setString (fps_phrase);
        text.setCharacterSize (48);
        text.setPosition (10.f, 10.f);

        if (!need_update) continue;
        printf ("the x offset %0.6lg\n", offs_re);
        printf ("the y offset %0.6lg\n", offs_im);
        printf ("the scale    %0.6lg\n", scale);

        printf("window draw start\n");
        need_update = false;
        window.clear ();
        window.draw (pixels);
        window.draw (text);
        window.display ();
        printf("window draw end\n");
    }

    return 0;
}

void SetPixelsColor (sf::VertexArray* pixels,
                     float offs_re, float offs_im, float scale)
{
    float general_offs_re = (OFFS_CENT_RE + offs_re) * scale;
    float general_offs_im = (OFFS_CENT_IM + offs_im) * scale;

    const int buf_size = 4;
    int X[buf_size] = {};
    int Y[buf_size] = {};

    for (int x = 0; x <= COLS_NUM - buf_size; x += buf_size) {
        for (int y = 0; y <= ROWS_NUM - buf_size; y += buf_size) {
            float X_0[4] = {};
            float Y_0[4] = {y};

            float re_0 = (scale * x - general_offs_re) * 4 / COLS_NUM;
            float im_0 = (scale * y - general_offs_im) * 4 / ROWS_NUM;
            float re_n = 0, im_n = 0;

            int n = 0;
            while (RADIUS * RADIUS >= re_n * re_n + im_n * im_n
                   && n < NUM_TO_STOP_CALC) {
                float tmp = re_n;
                re_n = re_n * re_n - im_n * im_n + re_0;
                im_n = 2 * tmp * im_n + im_0;

                ++n;
            }

            if (n >= NUM_TO_STOP_CALC) {
                sf::Color color = sf::Color::Black;
                (*pixels)[x * COLS_NUM + y] = sf::Vertex (sf::Vector2f (x, y), color);

                continue;
            }

            sf::Color color = sf::Color (n, (n * 210) % 256, (n * 123) % 256);
            (*pixels)[x * COLS_NUM + y] = sf::Vertex (sf::Vector2f (x, y), color);
        }
    }
}

void TransformView (float* x_coord, float* y_coord, int step,
                    float* scale, bool* need_update)
{
    if (IS_PRESSED(A)) {*x_coord += step; *need_update = true;}
    if (IS_PRESSED(D)) {*x_coord -= step; *need_update = true;}
    if (IS_PRESSED(S)) {*y_coord -= step; *need_update = true;}
    if (IS_PRESSED(W)) {*y_coord += step; *need_update = true;}

    if (IS_PRESSED(Equal)) {
        *scale /= SCALE_MULTIPLIER;
        *need_update = true;
    }

    if (IS_PRESSED(Dash)) {
        *scale *= SCALE_MULTIPLIER;
        *need_update = true;
    }
}
