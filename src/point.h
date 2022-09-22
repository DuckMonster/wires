#pragma once

struct Point
{
	i32 x;
	i32 y;

	Point() : x(0), y(0) {}
	Point(i32 x, i32 y) : x(x), y(y) {}

	bool operator==(const Point& rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Point& rhs) const { return x != rhs.x || y != rhs.y; }

	Point operator+(const Point& rhs) const { return Point(x + rhs.x, y + rhs.y); }
};