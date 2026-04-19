#include <SFML/Graphics.hpp>
#include <vector>

#define COLS_NUM 750
#define ROWS_NUM 750
#define RADIUS 2

int main()
{
    sf::RenderWindow window(sf::VideoMode(COLS_NUM, ROWS_NUM), "Mandelbrot");

    std::vector<sf::Vertex> pixels(COLS_NUM * ROWS_NUM);
    double offset_re = COLS_NUM / 2, offset_im = ROWS_NUM / 2;

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

            if (n <= 255){
                pixels[x * COLS_NUM + y] =
                    sf::Vertex (sf::Vector2f (x, y),
                                sf::Color (n, (n * 210) % 256, (n * 123) % 256));
            }
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        window.clear();
        window.draw (pixels.data (), pixels.size (), sf::Points);
        window.display();
    }

    return 0;
}
