/*#include <SFML\Graphics.hpp>
#include "DrawingMap.h"
#include "FloatMap.h"

const int WINDOW_WIDTH = 400, WINDOW_HEIGHT = 200;
sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML works!");

int drawMode = 0;

bool isWindowOpen()
{
	return window.isOpen();
}

bool pollEvent(sf::Event e)
{
	return window.pollEvent(e);
}

void DrawGraphWindow(BoundXZ real_window, BoundXZ mind_window)
{
	/*sf::Image frame;
	frame.create(real_window.width, real_window.height, sf::Color::Blue);


	// Конечная отрисовка

	sf::Sprite sprite;
	sf::Texture texture;
	texture.loadFromImage(frame);
	sprite.setTexture(texture);
	sf::RectangleShape rsh(sf::Vector2f(real_window.width, real_window.height));
	rsh.setFillColor(sf::Color::Red);
	window.draw(rsh);
	window.display();
}*/