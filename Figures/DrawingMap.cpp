#pragma once
#include "SFML\Graphics.hpp"
#include "DrawingMap.h"
#include "FloatMap.h"
#include <string>

//Constants
const sf::Color IN_AREA(0, 120, 120, 255);
const sf::Color OUT_AREA(255, 255, 255, 255);
const sf::Color MARK_LINE(0, 0, 0, 128);
const sf::Color NUM_COLOR(0, 0, 0, 255);
const uint16_t AXIS_WIDTH = 3, MRKS = 10, MARK_BTW = 35, NUM_OFFSET = 5, NUM_SZ = 10;
const float MARK_SIZES[MRKS] = {0.05, 0.1, 0.5, 1, 2, 5, 10, 25, 50, 100};
// Global
sf::RenderWindow window;
sf::Uint8* graphPixels;
sf::Font num_font;
uint16_t currentWidth, currentHeight, videoWidth, videoHeight;

DrawingMap::DrawingMap(uint16_t width, uint16_t height, char* programName)
{
	window.create(sf::VideoMode(width, height), programName);
	currentWidth = videoWidth = width;
	currentHeight = videoHeight = height;
	graphPixels = new sf::Uint8[width * height * 4];
	num_font.loadFromFile("arial.ttf");
}

sf::RenderWindow* DrawingMap::getWindow()
{
	return &window;
}

void DrawingMap::drawLines(float x, float y, sf::Vector2f lw, Box real_window, Box mind_window, sf::Color c, sf::Vector2f step, sf::Vector2f scale)
{
	int16_t i = 0;
	sf::RectangleShape line;
	while ((step.x >= 0 || x >= mind_window.x) && (step.x <= 0 || x < mind_window.width + mind_window.x)
		&& (step.y >= 0 || y >= mind_window.y) && (step.y <= 0 || y < mind_window.height + mind_window.y))
	{
		float box_w = lw.x,
			box_h = lw.y;
		line.setSize(sf::Vector2f(box_w, box_h));
		float real_x = (x - mind_window.x) * real_window.width / mind_window.width;
		float real_y = (mind_window.height + mind_window.y - y) * real_window.height / mind_window.height;
		line.setPosition(real_x * scale.x, real_y * scale.y);
		line.setFillColor(c);
		line.setScale(scale);
		window.draw(line);
		x += step.x, y += step.y, ++i;
	}
}

void DrawingMap::drawNumbers(float x, float y, Box real_window, Box mind_window, sf::Vector2f step, sf::Vector2f scale, bool axis)
{
	while ((step.x >= 0 || x >= mind_window.x) && (step.x <= 0 || x < mind_window.width + mind_window.x)
		&& (step.y >= 0 || y >= mind_window.y) && (step.y <= 0 || y < mind_window.height + mind_window.y))
	{
		x += step.x, y += step.y;
		float real_x = (x - mind_window.x) * real_window.width / mind_window.width;
		float real_y = (mind_window.height + mind_window.y - y) * real_window.height / mind_window.height;
		
		
		std::string name = std::to_string(axis ? round(x*100)/100 : ((int)round(y*100.f))*0.01f);
		while (name.size() > 2 && name[name.size() - 1] == '0')
		{
			name.pop_back();
			if (name[name.size() - 1] == '.') {
				name.pop_back();
				break;
			}
		}
		sf::Text text(name, num_font, NUM_SZ);
		text.setScale(scale);
		text.setPosition((real_x+NUM_OFFSET * axis) * scale.x, 
			(real_y + NUM_OFFSET * (!axis)) * scale.y);
		text.setFillColor(NUM_COLOR);
		window.draw(text);
	}
}

void DrawingMap::setPixel(uint16_t x, uint16_t y, size_t width, size_t height, sf::Color c)
{
	int k = (y * width + x) * 4;
	graphPixels[k] = c.r;
	graphPixels[k + 1] = c.g;
	graphPixels[k + 2] = c.b;
	graphPixels[k + 3] = c.a;
}

void DrawingMap::updateGraph(Box real_window, Box mind_window)
{
//Upd size
	sf::Vector2f scale = sf::Vector2f((float)videoWidth / real_window.width, (float)videoHeight / real_window.height);
	if (real_window.width != currentWidth || real_window.height != currentHeight)
	{
		//window.setSize(sf::Vector2u(real_window.width, real_window.height));
		currentWidth = real_window.width, currentHeight = real_window.height;
		graphPixels = new sf::Uint8[real_window.width * real_window.height * 4];
	}
// Make answer field
	float pix_delta = mind_window.width / real_window.width;
	
	for (size_t y = 0; y < real_window.height; ++y)
	{
		for (size_t x = 0; x < real_window.width; ++x)
		{
			float mind_x = mind_window.x + pix_delta * x;
			float mind_y = mind_window.y + pix_delta * (real_window.height - y - 1);

			if (isInArea(mind_x, mind_y))
				setPixel(x, y, real_window.width, real_window.height, IN_AREA);
			else setPixel(x, y, real_window.width, real_window.height, OUT_AREA);
		}
	}
// Make marklines
	// Y-axis
	sf::RectangleShape y_axis(sf::Vector2f(AXIS_WIDTH, real_window.height));
	y_axis.setFillColor(sf::Color::Black);
	y_axis.setPosition((-mind_window.x * real_window.width / mind_window.width - 1) * scale.x, 0);
	y_axis.setScale(scale);
	// X-axis
	sf::RectangleShape x_axis(sf::Vector2f(real_window.width, AXIS_WIDTH));
	x_axis.setFillColor(sf::Color::Black);
	x_axis.setPosition(0, ((mind_window.height + mind_window.y) * real_window.height / mind_window.height - 1) * scale.y);
	x_axis.setScale(scale);
	float btw_nums = MARK_SIZES[MRKS-1], btw_lines = MARK_SIZES[MRKS-2];
	for (int16_t i = 0; i < MRKS-1; ++i)
	{
		if (MARK_SIZES[i] / pix_delta >= MARK_BTW)
		{
			btw_lines = MARK_SIZES[i];
			int16_t j = i + 1;
			/*while (fmod(MARK_SIZES[j] / MARK_SIZES[i], 1.0) != 0)
				++j;*/ 
			btw_nums = MARK_SIZES[j];
			break;
		}
	}
// Make bounds
// Result drawing

	// image -> texture -> sprite -> shape
	sf::Texture graphTexture;
	graphTexture.create(real_window.width, real_window.height);
	graphTexture.update(graphPixels, real_window.width, real_window.height, 0, 0);
	sf::Sprite result_sprite; 
	result_sprite.setTexture(graphTexture);
	//Drawing options
	//result_sprite.setPosition(sf::Vector2f(real_window.x, real_window.y));
	result_sprite.setScale(scale);
	window.draw(result_sprite);
	window.draw(y_axis);
	window.draw(x_axis);
	drawLines(mind_window.x, 0, sf::Vector2f(real_window.width, 1), 
		real_window, mind_window, MARK_LINE, sf::Vector2f(0, -btw_lines), scale);
	drawLines(mind_window.x, 0, sf::Vector2f(real_window.width, 1),
		real_window, mind_window, MARK_LINE, sf::Vector2f(0, btw_lines), scale);
	drawLines(0, mind_window.y + mind_window.height, sf::Vector2f(1, real_window.height),
		real_window, mind_window, MARK_LINE, sf::Vector2f(-btw_lines, 0), scale);
	drawLines(0, mind_window.y + mind_window.height, sf::Vector2f(1, real_window.height),
		real_window, mind_window, MARK_LINE, sf::Vector2f(btw_lines, 0), scale);
	float num_x = std::max(mind_window.x, std::min(mind_window.x + mind_window.width - 3 * NUM_OFFSET * pix_delta, 0.f));
	float num_y = std::max(mind_window.y + 2.5f*NUM_OFFSET * pix_delta, std::min(mind_window.y + mind_window.height, 0.f));
	drawNumbers(num_x, 0, real_window, mind_window, sf::Vector2f(0, -btw_nums), scale, 0);
	drawNumbers(num_x, 0, real_window, mind_window, sf::Vector2f(0, btw_nums), scale, 0);
	drawNumbers(0, num_y, real_window, mind_window, sf::Vector2f(-btw_nums, 0), scale, 1);
	drawNumbers(0, num_y, real_window, mind_window, sf::Vector2f(btw_nums, 0), scale, 1);
	window.display();

}

DrawingMap::~DrawingMap() {}
