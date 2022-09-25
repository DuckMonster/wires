#pragma once

struct Point
{
	static Point component_min(Point lhs, Point rhs)
	{
		return Point(
			Math::min(lhs.x, rhs.x),
			Math::min(lhs.y, rhs.y)
		);
	}
	static Point component_max(Point lhs, Point rhs)
	{
		return Point(
			Math::max(lhs.x, rhs.x),
			Math::max(lhs.y, rhs.y)
		);
	}

	i32 x;
	i32 y;

	Point() : x(0), y(0) {}
	Point(i32 x, i32 y) : x(x), y(y) {}

	bool operator==(const Point& rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Point& rhs) const { return x != rhs.x || y != rhs.y; }

	Point operator+(const Point& rhs) const { return Point(x + rhs.x, y + rhs.y); }
	Point operator-(const Point& rhs) const { return Point(x - rhs.x, y - rhs.y); }

	Point right() const { return Point(x + 1, y); }
	Point left() const { return Point(x - 1, y); }
	Point down() const { return Point(x, y + 1); }
	Point up() const { return Point(x, y - 1); }
};