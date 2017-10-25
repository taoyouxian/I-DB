#pragma once

#ifndef FILEOPT_H_INCLUDED
#define FILEOPT_H_INCLUDED

#include "common.h"
#include "pageOpt.h"
#include "segmentOpt.h"
#include "bufOpt.h"
#include "Test.h"

#include <Polygon.h>
#include <vector>
#include <QString> 

/* (�ļ���Ϣ)
*/
struct FileInfo{
	int fileType;                // ��ʶ�Ƿ�Ϊ�����ļ����Լ������ļ������ͣ�hash/b��/˳��,���ԣ�
	int fileID;                  // �ļ���
	int fileName;                // �ļ���
	int fileState;                // �ļ�״̬
	int fileSegNum;              // �ļ�ռ���˶��ٶ�
	struct Segment segTable[SEG_NUM];		//	���������
};

struct Point;

class FileOpt
{
public:
	FileOpt(void);
	~FileOpt(void);

public:
	void		SavePolyAsFile(std::vector< std::vector<Point*> >&		m_points);		//	����Ϊ�ļ�
	void		ReadFiletoPoints(std::vector< std::vector<Point*> >&		m_points);	//  ���ļ�
	void		LoadData(vector<std::vector<supplier*>> suppliers);	//  LoadData


	int createFile(struct DbMetaHead *head, int type, long requestPageNum);
	void readFile(struct DbMetaHead *head, int fileID, char *des);
	void writeFile(struct DbMetaHead *head, int fileID, int length, char *str);
	void deleteFile(struct DbMetaHead *head, int fileID);
	int queryFileByIndex(struct DbMetaHead *head, int fileID);

	PageOpt pageOpt;
	BufOpt bufOpt;

private:
	QString		OpenFileDialog();
	QString		SaveFileDialog();
};


#endif // PAGEOPT_H_INCLUDED