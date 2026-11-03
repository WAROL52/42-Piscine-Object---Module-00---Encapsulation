#include "Vector2.hpp"

Vector2::Vector2() : x(0.0f), y(0.0f)
{
}

Vector2::Vector2(float px, float py) : x(px), y(py)
{
}

Vector2::Vector2(const Vector2& other) : x(other.x), y(other.y)
{
}

Vector2& Vector2::operator=(const Vector2& other)
{
	if (this != &other)
	{
		x = other.x;
		y = other.y;
	}
	return (*this);
}

Vector2::~Vector2()
{
}
