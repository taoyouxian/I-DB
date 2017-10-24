#include "fileOpt.h"
#include <fstream>

#include <QFileDialog>
#include <QMessageBox>

using namespace std;

FileOpt::FileOpt(void)
{
}


FileOpt::~FileOpt(void)
{
}

QString FileOpt::OpenFileDialog() {
	QString fileName;  
	fileName = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open Config"), "", QObject::tr("Poly Files (*.poly)"));  

	if (!fileName.isNull())  
	{  
		return fileName;
	}  
	else  {
		return NULL;
	}
}

QString FileOpt::SaveFileDialog() {
	QString fileName;
	fileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save Config"), "", QObject::tr("Poly Files (*.poly)"));

	if (!fileName.isNull())
	{
		return fileName;
	}
	else {
		return NULL;
	}
}

void FileOpt::ReadFiletoPoints(std::vector< std::vector<Point*> >&		m_points)
{
	QString fileName = OpenFileDialog();
	if(fileName.isNull()) {
		;
	}
	else {
		Point *p = NULL;
		int x, y;
		std::vector<Point*> polygon;
		ifstream fin(fileName.toStdString());
		while (!fin.eof()) {
			fin >> x >> y;
			if(x + y > 0) {
				p = new Point(x, y);	// 在清除点的时候释放内存
				polygon.push_back(p);
			}
			else {
				m_points.push_back(polygon);
				polygon.clear();
			}
		}
		m_points.push_back(polygon);
		polygon.clear();
		fin.close();
	}
}

void FileOpt::SavePolyAsFile(std::vector< std::vector<Point*> >&		m_points)
{
	QString fileName = SaveFileDialog();
	if(fileName.isNull()) {
		;
	}
	else {
		ofstream fout(fileName.toStdString());
		for (int i = 0; i < m_points.size(); i ++) {
			for (int j = 0; j < m_points[i].size(); j ++) {
				fout << m_points[i][j]->x << "\t" << m_points[i][j]->y << endl;
			}
			fout << -1 << "\t" << -1 << endl;
		}
		fout.close();
		QMessageBox::about(NULL, QString::fromLocal8Bit("文件"), QString::fromLocal8Bit("保存成功"));
	}
}