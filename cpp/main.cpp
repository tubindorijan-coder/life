#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <bits/stdc++.h>

const int aSize = 640; //veličina grida aSize*aSize
const int scale = 2; //
const int winSize = aSize * scale;

const int running = 1; //da li pocinje pri pokretanju
const double dcs = 0.2; //vjerojatnost spontane emisije

std::vector<sf::Uint8> pixels(aSize * aSize * 4, 0);

double aut[3][aSize][aSize] = {};
double autCache[aSize][aSize] = {};

bool autInitialized = false;

double sumOfAll = 0;
double sumOfFrak = 0;

int startSum = 0;

int brojkoraka = 0;

void drawPixel(int red, int green, int blue, int alpha, int posX, int posY)
{
    std::size_t index = (posX * aSize + posY) * 4;
        pixels[index + 0] = red;
        pixels[index + 1] = green;
        pixels[index + 2] = blue;
        pixels[index + 3] = alpha;
}

void renderPixel(int x, int y, int br)
{
    if      (br == 1)    drawPixel(255, 0, 0, 255, x, y);
    else if (br == 2)    drawPixel(255, 75, 0, 255, x, y);
    else if (br == 3)    drawPixel(255, 125, 0, 255, x, y);
    else if (br == 4)    drawPixel(255, 175, 0, 255, x, y);
    else if (br == 5)    drawPixel(255, 255, 0, 255, x, y);
    else if (br == 6)    drawPixel(255, 255, 75, 255, x, y);
    else if (br == 7)    drawPixel(255, 255, 125, 255, x, y);
    else if (br == 8)    drawPixel(255, 255, 175, 255, x, y);
    else if (br > 8)     drawPixel(255, 255, 255, 255, x, y);
    else                 drawPixel(0, 0, 0, 255, x, y);
}

void initAut()
{
    autInitialized = true;

    for (int i = 0; i < aSize; i++)
	{
		for(int j = 0; j < aSize; j++)
		{
				aut[0][i][j] = (i > aSize*4/9 && j > aSize*4/9 && i < aSize*5/9 && j < aSize*5/9 ) ? 20 : 0;
				aut[1][i][j] = 0;
				aut[2][i][j] = 0;

				autCache[i][j] = -1;
		}
	}
}

void calcAndRender ()
{
    sumOfFrak = 0;
    sumOfAll = 0;
    double emisija = 0;

    for (int i = 0; i < aSize; i++)
    {
        for (int j = 0; j < aSize; j++)
        {
            if (aut[0][i][j] != autCache[i][j])
            {
                renderPixel(i, j, aut[0][i][j]);
                autCache[i][j] = aut[0][i][j];  //Spremi podatke o pixelu u autCache
            }

            sumOfAll += aut[0][i][j];
            sumOfFrak += aut[1][i][j];
            if(aut[1][i][j] > 1)    std::cout << "i= " << i << "j= " << j << "aut[1][i][j] " << aut[0][i][j] << std::endl;

            if(aut[1][i][j] > ((double)rand()) / RAND_MAX)   aut[0][i][j]++;     //Apsorpcija

            emisija = aut[1][i][j]/16;
            if( aut[0][i][j] >= 1 && dcs > ((double)rand()) / RAND_MAX){		//	Spontana emisija
				aut[0][i][j] = aut[0][i][j] - 1;
				emisija += 1.0/16.0;
			}

            aut[1][i][j]=0;	// Širenje vala
			if (emisija > 0){
				aut[1][i-1<0?aSize-1:i-1][j-1<0?aSize-1:j-1]=aut[1][i-1<0?aSize-1:i-1][j-1<0?aSize-1:j-1]+emisija;
				aut[1][i-1<0?aSize-1:i-1][j]				=aut[1][i-1<0?aSize-1:i-1][j]+emisija;
				aut[1][i-1<0?aSize-1:i-1][j+1>=aSize?0:j+1]	=aut[1][i-1<0?aSize-1:i-1][j+1>=aSize?0:j+1]+emisija;
				aut[1][i][j-1<0?aSize-1:j-1]				=aut[1][i][j-1<0?aSize-1:j-1]+emisija;
				aut[1][i][j+1>=aSize?0:j+1]					=aut[1][i][j+1>=aSize?0:j+1]+emisija;
				aut[1][i+1>=aSize?0:i+1][j-1<0?aSize-1:j-1]	=aut[1][i+1>=aSize?0:i+1][j-1<0?aSize-1:j-1]+emisija;
				aut[1][i+1>=aSize?0:i+1][j]					=aut[1][i+1>=aSize?0:i+1][j]+emisija;
				aut[1][i+1>=aSize?0:i+1][j+1>=aSize?0:j+1]	=aut[1][i+1>=aSize?0:i+1][j+1>=aSize?0:j+1]+emisija;
			}
        }
    }
    if(sumOfAll==0) std::cout << "Thermal death";
    if(startSum == 0)   startSum = sumOfAll;

    brojkoraka++;
}

int main() 
{
    sf::RenderWindow window(sf::VideoMode(winSize, winSize), "Life c++");

    sf::Texture texture;
    texture.create(aSize, aSize);
    sf::Sprite sprite(texture);
    sprite.setScale(static_cast<float>(scale), static_cast<float>(scale));

    srand(time(0));

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

        if (running == 1)
        {
            if (autInitialized == false)    initAut();
            calcAndRender();
        }

        texture.update(pixels.data());
        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}