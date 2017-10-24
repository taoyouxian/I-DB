#pragma once
#include "polygon.h"
#include <vector>
#include <list>

// 活化边
struct Edge {
	double	maxY;				// 最高点y坐标
	double	startX;				// 起点的x坐标
	double	inverseSlope;		// 斜率倒数

	// 重载方法，用于排序
	bool operator < (const Edge& other) const {
		return startX < other.startX;
	}
};

// 填充多边形		 
class FilledPolygon :
	public Polygon
{
public:
	FilledPolygon(void);
	~FilledPolygon(void);
	 
	std::vector<Edge>		m_AET;			// 活化边表<扫描线所交的边>
	std::list<Edge*>*		m_YList[2048];	// y桶
	
private:
	int maxY;			// 最高点y坐标
	int minY;			// 最低点y坐标
public:
	void BuildYList();    //建立Y桶 
	void ProcessScanLineFill(void (_stdcall *glVertex)(int, int));  //扫描线填充
	 
private:
	void AddEdge2YList(Point& a, Point& b);
	void UpdateAET(int lineY);
	void DrawOneLine(void (_stdcall *glVertex)(int, int), int LineY, int dist);
	void CleanYList() ; 
};