#include "Geometry.h"

Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}

Size::Size() : width(0), height(0) {};
Size::Size(int inx, int iny) : width(inx), height(iny) {};

Rect::Rect() : center(0, 0), size(0, 0) {};
Rect::Rect(int x, int y, int w, int h) {}
Rect::Rect(Vector2 & p, Size & s) : center(p), size(s) {};

const int Rect::Left() const
{
	return center.x - (size.width / 2);
}

const int Rect::Right() const
{
	return center.x + (size.width / 2);
}

const int Rect::Top() const
{
	return center.y - (size.height / 2);
}

const int Rect::Bottom() const
{
	return center.y + (size.height / 2);
}

const int Rect::Width() const
{
	return size.width;
}

const int Rect::Height() const
{
	return size.height;
}
