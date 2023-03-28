#pragma once
#include "Vector.h"

class Rect
{
public:
	Vector start;
	Vector end;

public:
	Rect();
	Rect(Vector start, Vector end);
	Rect(int lx, int ly, int rx, int ry);

	int GetWidth();
	int GetHeight();

	bool operator ==(const Rect& other)
	{
		return (this->start == other.start && this->end == other.end);
	}

	bool operator !=(const Rect& other)
	{
		return (this->start != other.start || this->end != other.end);
	}
};

