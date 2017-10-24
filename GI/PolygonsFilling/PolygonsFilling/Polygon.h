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

// ����θ���			 
class Polygon
{
public:
	Polygon(void);
	virtual ~Polygon(void);

	std::vector<Point*>		m_points;	// ��������
};

