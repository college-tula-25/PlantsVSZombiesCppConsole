#pragma once

#include <iostream>

class Vector2
{
private:
	int x{ 0 };
	int y{ 0 };

public:
	Vector2() = default;
	Vector2(int x, int y) : x(x), y(y) {}

	Vector2 operator-() const { return { -(this->x), -(this->y) }; }

	Vector2 operator+(const Vector2& other) const { return { this->x + other.x, this->y + other.y }; }
	Vector2 operator-(const Vector2& other) const { return { this->x - other.x, this->y - other.y }; }

	Vector2 operator*(int scalar) const { return { x * scalar, y * scalar }; }
	Vector2 operator/(int scalar) const { return { x / scalar, y / scalar }; }

	Vector2& operator+=(const Vector2& other) {
		this->x += other.x;
		this->y += other.y;
		return *this;
	}
	Vector2& operator-=(const Vector2& other) {
		this->x -= other.x;
		this->y -= other.y;
		return *this;
	}
	Vector2& operator*=(const Vector2& other) {
		this->x *= other.x;
		this->y *= other.y;
		return *this;
	}

	bool operator==(const Vector2& other) const { return x == other.x && y == other.y; }
	bool operator!=(const Vector2& other) const { return !(*this == other); }

	int GetX() const { return x; }
	int GetY() const { return y; }

	double Length() const;
	double DistanceTo(const Vector2&) const;

	friend std::ostream& operator<<(std::ostream& os, const Vector2& vec) {
		os << "(" << vec.x << ", " << vec.y << ")\n";
		return os;
	}
};

inline Vector2 operator*(int scalar, const Vector2& vec) { return vec * scalar; }