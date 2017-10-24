#pragma once

#include <vector>

struct Point {
	int x;
	int y;
	Point(int a, int b):x(a), y(b) {}
	Point(){}
};
struct Line {
	Point start;
	Point end;
	Line(Point a, Point b):start(a), end(b) {}
	Line(){}
};

// 多边形父类			 
class Polygon
{
public:
	Polygon(void);
	virtual ~Polygon(void);

	std::vector<Point*>		m_points;	// 顶点序列
};

