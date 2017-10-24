#pragma once
#include <Polygon.h>
#include <vector>
#include <QString>
struct Point;
class FileOpt
{
public:
	FileOpt(void);
	~FileOpt(void);

public:
	void		SavePolyAsFile(std::vector< std::vector<Point*> >&		m_points);		//	����Ϊ�ļ�
	void		ReadFiletoPoints(std::vector< std::vector<Point*> >&		m_points);	//  ���ļ�
private:
	QString		OpenFileDialog();
	QString		SaveFileDialog();
};

