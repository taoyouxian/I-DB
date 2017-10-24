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
	void		SavePolyAsFile(std::vector< std::vector<Point*> >&		m_points);		//	保存为文件
	void		ReadFiletoPoints(std::vector< std::vector<Point*> >&		m_points);	//  读文件
private:
	QString		OpenFileDialog();
	QString		SaveFileDialog();
};

