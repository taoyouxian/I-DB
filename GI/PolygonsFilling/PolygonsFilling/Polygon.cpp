#include "Polygon.h"


Polygon::Polygon(void)
{

}


Polygon::~Polygon(void)
{
	for (int i = 0; i < m_points.size(); i ++) {
		delete m_points[i];
		m_points[i] = nullptr;
	}
}
