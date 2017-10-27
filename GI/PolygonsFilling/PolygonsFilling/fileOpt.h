#pragma once

#ifndef FILEOPT_H_INCLUDED
#define FILEOPT_H_INCLUDED

#include "common.h"
#include "pageOpt.h"
#include "segmentOpt.h"
#include "bufOpt.h"
#include "Test.h"
 
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

class MainWindow;

class FileOpt
{
public:
	FileOpt(void);
	~FileOpt(void);

public:
	void		SavePolyAsFile();		//	����Ϊ�ļ�
	void		ReadFiletoPoints();	//  ���ļ�
	void		LoadData(std::vector<supplier*> &suppliers);	//  LoadData


	int createFile(struct DbMetaHead *head, int type, long requestPageNum, MainWindow* ui);
	void readFile(struct DbMetaHead *head, int fileID, char *des, MainWindow* ui);
	void readFile(struct DbMetaHead *head, int fileID, char *des, MainWindow* ui, int pageNo);
	void writeFile(struct DbMetaHead *head, int fileID, int length, char *str, MainWindow* ui);
	void deleteFile(struct DbMetaHead *head, int fileID, MainWindow* ui);
	int queryFileByIndex(struct DbMetaHead *head, int fileID, MainWindow* ui);

	PageOpt pageOpt;
	BufOpt bufOpt;

private:
	QString		OpenFileDialog();
	QString		SaveFileDialog();
	vector<std::string>		InfoSplit(const string& s, const string& c);
};


#endif // PAGEOPT_H_INCLUDED