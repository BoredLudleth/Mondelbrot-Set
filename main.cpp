#include "drawPic.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000, true), "Mondelbrot-Set");     //potential sick fall if doesn't make

    unsigned char pixels[4 * 1000 * 1000];

    float x0 = -3;
    float y0 = 3;
    float length = 6;

    clock_t begin = 0;
    clock_t end = 0;
 
    countPix (pixels, x0, y0, length);

    sf::Texture mondelbrot;
    mondelbrot.create(1000, 1000);
    mondelbrot.update(pixels, 1000, 1000, 0, 0);
 
    sf::Sprite sprite(mondelbrot);
    sprite.setPosition(0,0);
 
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            begin = clock();
            
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Right) {
                    x0 += length / 10;
                    updateFrame (&mondelbrot, pixels, x0, y0, length);
                }

                if (event.key.code == sf::Keyboard::Left) {
                    x0 -= length / 10;
                    updateFrame (&mondelbrot, pixels, x0, y0, length);
                }

                if (event.key.code == sf::Keyboard::Up) {
                    y0 += length / 10;
                    updateFrame (&mondelbrot, pixels, x0, y0, length);
                } 
                
                if (event.key.code == sf::Keyboard::Down) {
                    y0 -= length / 10;
                    updateFrame (&mondelbrot, pixels, x0, y0, length);
                }

                if (event.key.code == sf::Keyboard::F1) {
                    length /= 1.3;
                    updateFrame (&mondelbrot, pixels, x0, y0, length);
                }

                if (event.key.code == sf::Keyboard::F2) {
                    length *= 1.3; 
                    updateFrame (&mondelbrot, pixels, x0, y0, length);
                }
            }
            if (event.type != sf::Event::KeyPressed) {
                updateFrame (&mondelbrot, pixels, x0, y0, length);
            }
            if ( event.type == sf::Event::Closed )
            {
                window.close();
                break;
            }



            end = clock();
            system ("clear");
            printf ("FPS: %lf\n", (double) (1.0 / ((double) (end - begin) / CLOCKS_PER_SEC)));
        }
 
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
