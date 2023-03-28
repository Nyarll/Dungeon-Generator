#include "Vector.h"

const Vector Vector::zero = Vector();

Vector::Vector()
{
	this->x = 0;
	this->y = 0;
}

Vector::Vector(int x, int y)
{
	this->x = x;
	this->y = y;
}

Vector::Vector(const Vector& other)
{
	this->x = other.x;
	this->y = other.y;
}
