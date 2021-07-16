#include <SFML\Graphics.hpp> 
#include "DrawingMap.h"
#include <iostream>
#include <fstream>

// Constants
float SCROLL_OFFSET = 0.9f;
// Global
Box mindFrame(-6, -6, 12, 12), frame(0, 0, 228, 228);
char * programName = "Figures";
bool mouseLeftPressed = false;
int mouseX = 0, mouseY = 0;
DrawingMap drawingMap(frame.width, frame.height, programName);

int main()
{
	sf::RenderWindow* window = drawingMap.getWindow();
	while (window->isOpen())
	{
		sf::Event e;
		while (window->pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
				window->close();
			else if (e.type == sf::Event::Resized)
			{
				float delta = (e.size.height + 0.f - frame.height) * mindFrame.height / frame.height;
				mindFrame.y -= delta;
				mindFrame.height += delta;
				mindFrame.width += (e.size.width + 0.f - frame.width) * mindFrame.width / frame.width;
				frame.width = e.size.width;
				frame.height = e.size.height;
			}
			else if (e.type == sf::Event::MouseWheelScrolled)
			{
				float offset = powf(SCROLL_OFFSET, e.mouseWheelScroll.delta);
				float dx = mindFrame.width * offset - mindFrame.width;
				float dy = mindFrame.height * offset - mindFrame.height;
				mindFrame.x -= e.mouseWheelScroll.x * dx / frame.width;
				mindFrame.y -= (frame.height - e.mouseWheelScroll.y) * dy / frame.height;
				mindFrame.width *= offset;
				mindFrame.height *= offset;
			}
			else if (e.type == sf::Event::MouseButtonPressed)
			{
				if (e.mouseButton.button == sf::Mouse::Left)
					mouseLeftPressed = true;
				mouseX = e.mouseButton.x;
				mouseY = e.mouseButton.y;
			}
			else if (e.type == sf::Event::MouseButtonReleased)
			{
				if (e.mouseButton.button == sf::Mouse::Left)
					mouseLeftPressed = false;
				mouseX = e.mouseButton.x;
				mouseY = e.mouseButton.y;
			}
			else if (e.type == sf::Event::MouseEntered)
			{
				mouseX = e.mouseButton.x;
				mouseY = e.mouseButton.y;
			}
			else if (e.type == sf::Event::MouseMoved)
			{
				if (mouseLeftPressed)
				{
					float delta_x = e.mouseMove.x - mouseX;
					float delta_y = e.mouseMove.y - mouseY;
					mindFrame.x -= delta_x / frame.width * mindFrame.width;
					mindFrame.y += delta_y / frame.height * mindFrame.height;
				}
				mouseX = e.mouseMove.x;
				mouseY = e.mouseMove.y;
			}
		}
		drawingMap.updateGraph(frame, mindFrame);
	}
	
	return 0;
}
// sfml-graphics.lib; sfml-system.lib; sfml-window.lib; sfml-audio.lib;