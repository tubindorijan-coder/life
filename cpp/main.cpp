#include <SFML/Graphics.hpp>
#include <vector>

const int aSize = 640; //veličina grida aSize*aSize
const int scale = 1; //
const int winSize = aSize * scale;

std::vector<sf::Uint8> pixels(aSize * aSize * 4, 0);

void drawPixel(int red, int green, int blue, int alpha, int posX, int posY)
{
    std::size_t index = (posX * aSize + posY) * 4;
        pixels[index + 0] = red;
        pixels[index + 1] = green;
        pixels[index + 2] = blue;
        pixels[index + 3] = alpha;
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(winSize, winSize), "Life c++");

    sf::Texture texture;
    texture.create(aSize, aSize);
    sf::Sprite sprite(texture);
    sprite.setScale(static_cast<float>(scale), static_cast<float>(scale));

    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed) 
            {
                window.close();
            }
        }

        for (int i = 0; i < aSize; i++)
        {
            for (int j = 0; j < aSize; j++)
            {
                drawPixel(i, j, i - j, 250, i, j);
            }
        }

        texture.update(pixels.data());
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}