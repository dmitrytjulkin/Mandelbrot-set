#include <SFML/Graphics.hpp>
#include <vector>
#include <unistd.h>

const int COLS_NUM     = 500;
const int ROWS_NUM     = 500;
const int OFFS_CENT_RE = 250;
const int OFFS_CENT_IM = 250;
const int RADIUS       = 2;

const int SCALE_MULTIPLIER = 2;
const int VIEW_OFFSET_VAL  = 50;

const int NUM_TO_STOP_CALC = 256;

void TransformView  (float* x_coord, float* y_coord, int step, float* scale);
void SetPixelsColor (/*sf::Image* image, */sf::VertexArray* pixels,
                     float offs_re, float offs_im, float scale);

int main ()
{
    // sf::Image image = {};
    // image.create(COLS_NUM, ROWS_NUM, sf::Color::Black);
    sf::RenderWindow window (sf::VideoMode (COLS_NUM, ROWS_NUM), "Mandelbrot");
    printf("Window created\n");

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

    while (window.isOpen ())
    {
        printf("set pixel start\n");
        SetPixelsColor (/*&image,*/ &pixels, offs_re, offs_im, scale);
        printf("set pixel end\n");

        // if (event.type == sf::Event::Closed)
        //     window.close();

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        //     printf ("A PRESSED\n");

        printf("check keyboard\n");

        TransformView (&offs_re, &offs_im, VIEW_OFFSET_VAL, &scale);

        delta_time = clock.restart().asSeconds();
        fps = 1 / delta_time;
        printf("fps = %.2lg\n", fps);

        char fps_phrase[21] = {};
        sprintf (fps_phrase, "FPS = %.2lg", fps);
        text.setFont (arial_font);
        text.setString (fps_phrase);
        text.setCharacterSize (48);
        text.setPosition (10.f, 10.f);

        printf("window draw start\n");
        window.clear ();
        window.draw (pixels);
        window.draw (text);
        window.display ();
        printf("window draw end\n");
    }

    return 0;
}

void SetPixelsColor (/*sf::Image* image,*/ sf::VertexArray* pixels,
                     float offs_re, float offs_im, float scale)
{
    float general_offs_re = OFFS_CENT_RE + offs_re * scale;
    float general_offs_im = OFFS_CENT_IM + offs_im * scale;

    for (int x = 0; x < COLS_NUM; ++x) {
        for (int y = 0; y < ROWS_NUM; ++y) {
            float re_0 = (x - general_offs_re) * 4 / COLS_NUM;
            float im_0 = (y - general_offs_re) * 4 / ROWS_NUM;
            float re_n = 0, im_n = 0;

            int n = 0;
            while (RADIUS * RADIUS >= re_n * re_n + im_n * im_n
                   && n < NUM_TO_STOP_CALC) {
                float tmp = re_n;
                re_n = re_n * re_n - im_n * im_n + re_0;
                im_n = 2 * tmp * im_n + im_0;

                ++n;
            }

            if (n >= NUM_TO_STOP_CALC)
                continue;

            sf::Color color = sf::Color (n, (n * 210) % 256, (n * 123) % 256);
            (*pixels)[x * COLS_NUM + y] =
                sf::Vertex (sf::Vector2f (x, y), color);

            // image->setPixel (x - offs_re, y - offs_im, color);
        }
    }
}

void TransformView (float* x_coord, float* y_coord, int step, float* scale)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) *x_coord -= step;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) *x_coord += step;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) *y_coord -= step;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) *y_coord += step;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
        *scale *= SCALE_MULTIPLIER;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
        *scale *= 1 / SCALE_MULTIPLIER;
}
