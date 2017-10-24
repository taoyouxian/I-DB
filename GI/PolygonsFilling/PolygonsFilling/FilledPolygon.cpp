#include "FilledPolygon.h"
#include <algorithm>

FilledPolygon::FilledPolygon(void)
{
	for (int i = 0; i < 2048; i ++) {
		m_YList[i] = nullptr;
	}
	minY = 2048;
	maxY = 0;
}
FilledPolygon::~FilledPolygon(void)
{
	if(this != nullptr) {
		CleanYList();
	}
}
void FilledPolygon::CleanYList() {
	std::list<Edge*>::iterator iter;
	for (int i = minY; i <= maxY; i ++) {
		if(m_YList[i] != nullptr) {
			iter = m_YList[i]->begin();
			while(iter != m_YList[i]->end()) {
				delete *iter;
				iter ++;
			}
			m_YList[i]->clear();
			delete m_YList[i];
			m_YList[i] =nullptr;
		}
	}
	minY = 2048;
	maxY = 0;
} 
//y桶
void FilledPolygon::BuildYList() {
	CleanYList();
	minY = 2048;
	maxY = 0;
	int sz = m_points.size();
	Point point1, point2;
	for(int i = 0; i < sz; i ++) {
		point1 = *m_points[i%sz];
		point2 = *m_points[(i+1)%sz];
		if(point1.y == point2.y) {
			continue;
		}
		if(point1.y > point2.y)	{
			AddEdge2YList(point1, point2);
		}
		else if(point1.y < point2.y){
			AddEdge2YList(point2, point1);
		}
	}
}
// 将边加入“y桶”中
void FilledPolygon::AddEdge2YList(Point& p1, Point& p2) {
	if(m_YList[p2.y] == NULL) {
		m_YList[p2.y] = new std::list<Edge*>();//初始化Y值
	} 
	Edge *e = new Edge();
	e->startX = p2.x;
	e->maxY = p1.y - 1;	// 挖掉上顶点 
	e->inverseSlope = (p1.x - p2.x) * 1.0 / (p1.y - p2.y);
	m_YList[p2.y]->push_back(e);

	if(minY > p2.y) {minY = p2.y;}
	if(maxY < p1.y) {maxY = p1.y;}
}
//扫描线
void FilledPolygon::ProcessScanLineFill(void (_stdcall *glVertex)(int, int))
{
	std::list<Edge*> *EdgeList = nullptr;
	std::list<Edge*>::iterator iter; 
	for (int i = minY; i <= maxY; i ++) {
		EdgeList = m_YList[i];
		if(EdgeList != nullptr) {
			iter = EdgeList->begin();
			while(iter != EdgeList->end()) {
				m_AET.push_back(**iter);
				iter ++;
			}
		}
		UpdateAET(i); //更新AEL
		DrawOneLine(glVertex, i, 1);//连线
	}
}
void FilledPolygon::UpdateAET(int lineY) {
	std::vector<Edge>::iterator iter = m_AET.begin();
	while(iter != m_AET.end()) {
		Edge *e = &(*iter);
		e->startX += e->inverseSlope;
		if(lineY > e->maxY) {
			iter = m_AET.erase(iter);
		}
		else {
			iter ++;
		}
	}
}
void FilledPolygon::DrawOneLine(void (_stdcall *glVertex)(int, int), int LineY, int dist)
{ 
	if((LineY - minY) % dist != 0) {
		return ;
	}
	int sz = m_AET.size();
	for (int i = 0; i < sz - 1; i += 2) {
		int start = m_AET[i].startX;
		int end = m_AET[i+1].startX; 
		glVertex(start, LineY);
		glVertex(end, LineY);
	}
} 