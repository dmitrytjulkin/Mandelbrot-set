#include <SFML/Graphics.hpp>
#include <vector>

#define COLS_NUM 750
#define ROWS_NUM 750
#define RADIUS 2

void SetPixelsColor (std::vector<sf::Vertex>* pixels,
                     double offset_re, double offset_im);

// int main ()
// {
//     sf::RenderWindow window (sf::VideoMode (1280, 720), "Smth");
//     window.setFramerateLimit (60);
//
//     sf::Font arial_font = {};
//     arial_font =
// }


int main ()
{
    sf::RenderWindow window (sf::VideoMode (COLS_NUM, ROWS_NUM), "Mandelbrot");

    std::vector<sf::Vertex> pixels (COLS_NUM * ROWS_NUM);
    double offset_re = COLS_NUM / 2, offset_im = ROWS_NUM / 2;

    sf::Event event = {};
    sf::Clock clock = {};
    sf::Text text = {};

    sf::Font arial_font = {};
    arial_font.loadFromFile ("/root/TDA projects/Mandelbrot-set/arial.ttf");

    double last_time = 0, current_time = 0;
    double fps = 0;

    while (window.isOpen ())
    {
        SetPixelsColor (&pixels, offset_re, offset_im);

        while (window.pollEvent (event))
            if (event.type == sf::Event::Closed)
                window.close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) offset_re -= 50;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) offset_re += 50;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) offset_im -= 50;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) offset_im += 50;

        current_time = clock.restart().asSeconds();
        fps = 1 / (current_time - last_time);
        last_time = current_time;


        char fps_phrase[21] = {};
        sprintf (fps_phrase, "FPS = %.2lg", fps);
        text.setFont (arial_font);
        text.setString (fps_phrase);
        text.setCharacterSize (48);
        text.setPosition(10.f, 10.f);

        window.clear ();
        window.draw (pixels.data (), pixels.size (), sf::Points);
        window.draw (text);
        window.display ();
    }

    return 0;
}

void SetPixelsColor (std::vector<sf::Vertex>* pixels, double offset_re, double offset_im)
{
    for (int x = 0; x < COLS_NUM; ++x) {
        for (int y = 0; y < ROWS_NUM; ++y) {
            double re_0 = (x - offset_re) * 4 / COLS_NUM;
            double im_0 = (y - offset_im) * 4 / ROWS_NUM;
            double re_n = 0, im_n = 0;

            int n = 0;
            while (n < 256 && RADIUS * RADIUS >= re_n * re_n + im_n * im_n) {
                double tmp = re_n;
                re_n = re_n * re_n - im_n * im_n + re_0;
                im_n = 2 * tmp * im_n + im_0;

                ++n;
            }

            if (n > 255)
                continue;

            (*pixels)[x * COLS_NUM + y] =
                sf::Vertex (sf::Vector2f (x, y),
                            sf::Color (n, (n * 210) % 256, (n * 123) % 256));
        }
    }
}
