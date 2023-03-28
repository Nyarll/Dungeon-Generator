#include "Rect.h"

Rect::Rect()
{
	this->start = Vector::zero;
	this->end = Vector::zero;
}

Rect::Rect(Vector start, Vector end)
{
	this->start = start;
	this->end = end;
}

Rect::Rect(int lx, int ly, int rx, int ry)
{
	this->start = Vector(lx, ly);
	this->end = Vector(rx, ry);
}

int Rect::GetWidth()
{
	return this->end.x - this->start.x;
}

int Rect::GetHeight()
{
	return this->end.y - this->start.y;
}
