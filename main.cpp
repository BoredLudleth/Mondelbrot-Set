#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000, true), "Mondelbrot-Set");

    sf::Uint8 pixels[4 * 1000 * 1000];

    float x0 = -3;
    float y0 = 3;
    float dx = 0.006;
    printf ("dx %f\n", dx);
    float max_r = 100;
    int counter = 0;

    float x_2 = 0;
    float y_2 = 0;
    float xy = 0;

    int current_p = 0;
 
    for (int pix_y = 0; pix_y < 1000; pix_y++) {
        y0 -= dx;
        x0 = -3;
        for (int pix_x = 0; pix_x < 1000; pix_x++, current_p += 4) {
            x0 += dx;

            for (float x_n = x0, y_n = y0; counter < 255; counter++) {
                x_2 = x_n * x_n;
                y_2 = y_n * y_n;
                xy = x_n * y_n;

                if ((x_2 + y_2) >= max_r) {
                    break;
                } else {
                    x_n = x_2 - y_2 + x0; 
                    y_n = xy + xy + y0; 
                }
            }

            pixels[current_p] = 255 - counter;
            pixels[current_p + 1] = (counter%4) * 64;
            pixels[current_p + 2] = 128;
            pixels[current_p + 3] = 255 - counter;
            counter = 0;
        }

    }

    sf::Texture mondelbrot;
    mondelbrot.create(1000,1000);
    mondelbrot.update(pixels,1000,1000,0,0);
 
    sf::Sprite sprite(mondelbrot);
    sprite.setPosition(0,0);
 
    while (window.isOpen())
    {
        sf::Event event;
        while ( window.pollEvent(event) )
        {
            // if (event.type == sf::Event::KeyPressed) {
            //     if (event.key.code == sf::Keyboard::Right) {
            //         x0 += 0.1;
            //     } else if (event.key.code == sf::Keyboard::Right) {
            //         x0 -= 0.1;
            //     } else if (event.key.code == sf::Keyboard::Up) {
            //         y0 += 0.1;
            //     } else if (event.key.code == sf::Keyboard::Up) {
            //         y0 -= 0.1;
            //     }
            // }
            if ( event.type == sf::Event::Closed )
            {
                window.close();
                break;
            }
        }
 
        window.clear();
        window.draw(sprite);
        window.display();
 
    }

    return 0;
}
// #include <chrono>
// #include <random>
// #include <SFML/Graphics.hpp>
 
 
// int main()
//     std::mt19937 gen ( std::chrono::system_clock::now().time_since_epoch().count() ) ;
//     std::uniform_int_distribution<sf::Uint32> distr ( 0 , std::numeric_limits<sf::Uint32>::max() ) ;
//     sf::RenderWindow window ( sf::VideoMode(800,600), "SFML" ) ;
 
//     sf::Uint32 arr[100][100] ;
 
//     for ( int x = 0 ; x < 100 ; ++x )
//         for ( int y = 0 ; y < 100 ; ++y )
//             arr[x][y] = distr(gen) ;
 
//     sf::Texture tx ;
//     tx.create(100,100) ;
//     tx.update((sf::Uint8*)arr,100,100,0,0);
 
//     sf::Sprite sprite(tx) ;
//     sprite.setPosition(50,50);
 
//     while (window.isOpen())
//     {
//         sf::Event event ;
//         while ( window.pollEvent(event) )
//         {
//             if ( event.type == sf::Event::Closed )
//             {
//                 window.close() ;
//                 break ;
//             }
//         }
 
//         window.clear( sf::Color::Red ) ;
//         window.draw(sprite) ;
//         window.display() ;
 
//     }
