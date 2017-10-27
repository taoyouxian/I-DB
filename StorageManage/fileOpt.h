#pragma once 

#ifndef FILEOPT_H_INCLUDED
#define FILEOPT_H_INCLUDED

#include "common.h"
#include "pageOpt.h"
#include "segmentOpt.h"
#include "bufOpt.h"


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

class FileOpt
{
public:
	FileOpt(void);
	~FileOpt(void);  
	 
	int createFile(struct DbMetaHead *head, int type, long requestPageNum);
	void readFile(struct DbMetaHead *head, int fileID, char *des);
	void writeFile(struct DbMetaHead *head, int fileID, int length, char *str);
	void deleteFile(struct DbMetaHead *head, int fileID);
	int queryFileByIndex(struct DbMetaHead *head, int fileID);

	PageOpt pageOpt;
	BufOpt bufOpt; 
}; 

#endif // PAGEOPT_H_INCLUDED