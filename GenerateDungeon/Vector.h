#pragma once
class Vector
{
public:
	static const Vector zero;

public:
	int x;
	int y;

public:
	// �R���X�g���N�^
	Vector();
	Vector(int x, int y);

	// �R�s�[�R���X�g���N�^
	Vector(const Vector& other);

	bool operator ==(const Vector& other)
	{
		return (this->x == other.x && this->y == other.y);
	}

	bool operator !=(const Vector& other)
	{
		return (this->x != other.x || this->y != other.y);
	}

};

