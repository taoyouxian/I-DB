#pragma once
#include "polygon.h"
#include <vector>
#include <list>

// ���
struct Edge {
	double	maxY;				// ��ߵ�y����
	double	startX;				// ����x����
	double	inverseSlope;		// б�ʵ���

	// ���ط�������������
	bool operator < (const Edge& other) const {
		return startX < other.startX;
	}
};

// �������		 
class FilledPolygon :
	public Polygon
{
public:
	FilledPolygon(void);
	~FilledPolygon(void);
	 
	std::vector<Edge>		m_AET;			// ��߱�<ɨ���������ı�>
	std::list<Edge*>*		m_YList[2048];	// yͰ
	
private:
	int maxY;			// ��ߵ�y����
	int minY;			// ��͵�y����
public:
	void BuildYList();    //����YͰ 
	void ProcessScanLineFill(void (_stdcall *glVertex)(int, int));  //ɨ�������
	 
private:
	void AddEdge2YList(Point& a, Point& b);
	void UpdateAET(int lineY);
	void DrawOneLine(void (_stdcall *glVertex)(int, int), int LineY, int dist);
	void CleanYList() ; 
};