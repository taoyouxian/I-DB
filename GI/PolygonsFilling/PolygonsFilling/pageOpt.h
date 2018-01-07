#pragma once

#ifndef PAGEOPT_H_INCLUDED
#define PAGEOPT_H_INCLUDED

#include "common.h"

/* (ҳ��ͷ����Ϣ)
*/ 
struct pageHead{
	long pageNo;                    // ҳ��
	int curRecordNum;               // ��ǰ��ҳ�洢�ļ�¼����
	long prePageNo;
	long nextPageNo;
	long freeSpace;                 // ��ҳ�Ŀ���ռ��С
};

/* (ҳ����) 
 * ���ڵ�ƫ������ӿ��ǰ���������
 * ���ڵļ�¼�ǴӺ���ǰ����
 */ 
struct offsetInPage{
	long logicID;       // �ü�¼��db�е��߼��ţ���Ϊ�߼���ַ��
	int recordID;       // �ü�¼�ڵ�ǰҳ�ļ�¼��
	int offset;         //�ü�¼����ڿ�β��ַ��ƫ����
	bool isDeleted;
};

class PageOpt
{
public:
	PageOpt();
	~PageOpt();
	 
	int getBit(unsigned long num, long pos);
	int setBit(unsigned long *num, long pos, int setValue);
	long allocatePage(struct dbSysHead *head, long requestPageNum);
	void recyOnePage(struct dbSysHead *head, long pageNo);
	void recyAllPage(struct dbSysHead *head);
}; 

#endif // PAGEOPT_H_INCLUDED