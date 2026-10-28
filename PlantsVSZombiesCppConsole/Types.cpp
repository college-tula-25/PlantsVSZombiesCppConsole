#pragma once

class Vector2
{
private:
	int x{ 0 };
	int y{ 0 };

public:
	Vector2() = default;
	Vector2(int x, int y) : x(x), y(y) {}

	int GetX() const { return x; }
	int GetY() const { return y; }

	Vector2& operator=(const Vector2& other) = default;
};
