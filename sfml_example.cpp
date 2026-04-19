#include <SFML/Graphics.hpp>

#define COLUMNS_NUM 200
#define ROWS_NUM 200
#define RADIUS 50

int main()
{
    sf::RenderWindow window(sf::VideoMode(COLUMNS_NUM, ROWS_NUM), "Mandelbrot");

    sf::Vertex vertices[COLUMNS_NUM * ROWS_NUM] = {};

    int re_p0 = COLUMNS_NUM / 2, im_p0 = ROWS_NUM / 2;

    for (int rows = 0; rows < COLUMNS_NUM; ++rows) {
        for (int cols = 0; cols < COLUMNS_NUM; ++cols) {
            int re_pn = rows + re_p0, im_pn = cols + im_p0;

            for (int n = 0; n < 256; ++n) {
                int re_pn1 = re_pn * re_pn - im_pn * im_pn + re_p0;
                int im_pn1 = 2 * re_pn * im_pn + im_p0;
                re_pn = re_pn1;
                im_pn = im_pn1;

                // if (n == 255)
                //    vertices[ROWS_NUM * (rows + re_p0) + (cols + im_p0)] = sf::Vertex(
                //             sf::Vector2f(cols + im_p0, rows + re_p0), sf::Color::Black);

                if (RADIUS * RADIUS < re_pn1 * re_pn1 + im_pn1 * im_pn1) {
                    vertices[ROWS_NUM * (rows + re_p0) + cols + im_p0] = sf::Vertex(sf::Vector2f(cols + im_p0, rows + re_p0), sf::Color (n, 255 - n, 0, 255));

                    break;
                }
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
        window.draw(vertices, COLUMNS_NUM * ROWS_NUM, sf::Points);
        window.display();
    }

    return 0;
}
