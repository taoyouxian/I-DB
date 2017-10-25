#pragma once

#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED

#include "ui_glwidget.h"
#include "common.h"
#include "fileOpt.h"
#include "bufOpt.h"
#include "pageOpt.h"
#include "segment.h"
#include "Test.h"
 
class MainWindow;

struct DbInfo{ 
	struct FileInfo fileInfo[MAX_FILE_NUM];	    //	��ÿһ���ļ���������

	long fileSize;                            //	�ļ����������Ĵ�С��Ĭ����Ϊ196MB
	long pageSize;                           //	ÿһ��ҳ�Ĵ�С��Ĭ��4KB
	long pageSum;                             //	�ܹ���ҳ��
	long pageFree;                              //	��ǰ�ж��ٿ��õ�ҳ  

	//bitMap ���пռ�λʾͼ
	long bitMapAddStart;                            //	�ļ���bitMap����ʼ��ַ
	long bitMapSize;                            //	bitMap�Ĵ�С�����ֽ�Ϊ��λ

	long dataAddStart;                              //  �ļ�������������ʼλ��
	int curFileNum;                            //	Ŀǰ�ж��ٸ��ļ������Ϊ MAX_FILE_NUM 

	long MapTable_firstPageNo;              //ӳ������ʼҳ��
	long curAvaiLogicID;                    //��ǰ��һ�����õ��߼���

};

struct DbMetaHead{
	struct DbInfo desc;
	unsigned long *FreeSpace_bitMap;
	struct bufferPool buff;
	FILE *dataPath;
};

//���£����ڴ��е���������д�뵽������
int memToDisk(struct DbMetaHead *head);

class Storage
{
private:
	vector<std::vector<supplier*>>		m_suppliers;

public:
	Storage();
	~Storage();
	 
	void createDbMetaHead(char *fileName); 
	void initDB(struct DbMetaHead *head, char *fileName, MainWindow *ui);
	void showDbInfo(struct DbMetaHead *head);
	void showSegTable(struct DbMetaHead *head, int fileID); 

	FileOpt fileOpt;
	BufOpt bufOpt;
	PageOpt pageOpt;
};

#endif // STORAGE_H_INCLUDED
