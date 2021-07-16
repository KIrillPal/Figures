#include <SFML\Graphics.hpp>
#include "FloatMap.h"

struct Parabola
{
	float a, b, c;
	Parabola(float a, float b, float c) : a(a), b(b), c(c) {}
	bool inXY(float x, float y)
	{
		if(a > 0)
			return ((a*x + b)*x + c <= y);
		else return ((a*x + b)*x + c >= y);
	}
	bool inYX(float x, float y)
	{
		if(a > 0)
			return ((a*y + b)*y + c <= x);
		else return ((a*y + b)*y + c >= x);
	}
};

struct Line
{
	float k, b;
	Line(float k, float b) : k(k), b(b) {};
	Line() {};
	bool below(float x, float y)
	{
		return (k*x+b >= y);
	}
	bool above(float x, float y)
	{
		return (k*x + b <= y);
	}
};

struct Rectangle
{
	float x0, y0, x1, y1;
	Rectangle(float x0, float y0, float x1, float y1)
		: x0(x0), y0(y0), x1(x1), y1(y1) {};
	bool in(float x, float y)
	{
		return ((x >= x0 && x <= x1)&&(y >= y0 && y <= y1));
	}
};

struct Circle
{
	float xc, yc, r;
	Circle(float xc, float yc, float r) : xc(xc), yc(yc), r(r) {};
	bool in(float x, float y)
	{
		return (((x - xc)*(x - xc) + (y - yc)*(y - yc)) <= r*r);
	}
};
struct Rhombus
{
	Line l1, l2, l3, l4;
	Rhombus(float h, float w, float xc, float yc)
	{
		float k = h / w, b = yc - xc * h / w;
		l1 = Line(-k, yc + xc * h / w + h);
		l2 = Line(k, yc - xc * h / w + h);
		l3 = Line(-k, yc + xc * h / w - h);
		l4 = Line(k, yc - xc * h / w - h);
	}
	bool in(float x, float y)
	{
		return l1.below(x, y)
			&& l2.below(x, y)
			&& l3.above(x, y)
			&& l4.above(x, y);
	}
};

Line a(-4.f/3, -10.f/3), b(-2, 10);
Circle c(3.5f, 4, 1.5f);
Rectangle d(-1, -1, 1.4, 1);
Rhombus e(1, 2, -2, -0.5);
Parabola f(14.f / 9, 70.f / 9, 65.f / 9), 
g(-10.f / 9, -10.f / 9, - 23.f / 9);

bool isInArea(float x, float y)
{
	bool area1 = 1;
	area1 &= a.below(x, y);
	area1 &= !e.in(x, y);
	area1 &= !f.inYX(x, y);
	area1 &= !g.inXY(x, y);
	area1 &= (x <= 0);
	bool area2 = 1;
	area2 &= a.below(x, y);
	area2 &= f.inYX(x, y);
	area2 &= g.inXY(x, y);
	bool area3 = 1;
	area3 &= d.in(x, y);
	area3 &= !e.in(x, y);
	bool area4 = 1;
	area4 &= b.above(x, y);
	area4 &= !c.in(x, y);
	bool area5 = 1;
	area5 &= b.below(x, y);
	area5 &= c.in(x, y);
	return area1 || area2 || area3 || area4 || area5;
}