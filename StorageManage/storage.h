#pragma once

#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED

#include "common.h"
#include "fileOpt.h"
#include "bufOpt.h"
#include "pageOpt.h"

struct buffBlock{
	long pageNo;
	long visitTime;
	bool isEdited;
};

struct buffSpace{
	struct buffBlock map[SIZE_BUFF];
	char data[SIZE_BUFF][SIZE_PER_PAGE];
	long curTimeStamp;
};

/* ���ڵ�ƫ������ӿ��ǰ���������
   ���ڵļ�¼�ǴӺ���ǰ����
*/
struct offsetInPage{
	int recordID;
	int offset;         //�ü�¼����ڿ�β��ַ��ƫ����
	bool isDeleted;
};

// 
struct pageHead{
	long pageNo;                    // ҳ��
	int curRecordNum;               // ��ǰ��ҳ�洢�ļ�¼����
	long prePageNo;
	long nextPageNo;
	long freeSpace;                 // ��ҳ�Ŀ���ռ��С
};

struct FileDesc{
	int fileType;               //��ʶ�Ƿ�Ϊ�����ļ����Լ������ļ������ͣ�hash/b��/˳��,���ԣ�
	int fileID;                    //�ļ���
	long fileFirstPageNo;
	long filePageNum;              //�ļ�ռ���˶���ҳ
};

struct SysDesc{
	long sizeOfFile;                            //	�ļ����������Ĵ�С��Ĭ����Ϊ196MB
	long sizePerPage;                           //	ÿһ��ҳ�Ĵ�С��Ĭ��4KB
	long totalPage;                             //	�ܹ���ҳ��
	long pageAvai;                              //	��ǰ�ж��ٿ��õ�ҳ

	//bitMap ���пռ�λʾͼ
	long bitMapAddr;                            //	�ļ���bitMap����ʼ��ַ
	long sizeBitMap;                            //	bitMap�Ĵ�С�����ֽ�Ϊ��λ

	long dataAddr;                              //  �ļ�������������ʼλ��
	int curFileNum;                            //	Ŀǰ�ж��ٸ��ļ������Ϊ MAX_FILE_NUM
	struct FileDesc fileDesc[MAX_FILE_NUM];		//	��ÿһ���ļ���������

	long MapTable_firstPageNo;              //ӳ������ʼҳ��
	long curAvaiLogicID;                    //��ǰ��һ�����õ��߼���

};

struct dbSysHead{
	struct SysDesc desc;
	unsigned long *FreeSpace_bitMap;
	struct buffSpace buff;
	FILE *fpdesc;
};

class Storage
{
public:
	Storage();
	~Storage();

	//initial.cpp
	void createSysHead(char *filename); 
	void init_database(struct dbSysHead *head, char *filename);
	void show_SysDesc(struct dbSysHead *head);
	void show_FileDesc(struct dbSysHead *head, int fid); 

	FileOpt fileOpt;
	BufOpt bufOpt;
	PageOpt pageOpt;
};

#endif // STORAGE_H_INCLUDED
