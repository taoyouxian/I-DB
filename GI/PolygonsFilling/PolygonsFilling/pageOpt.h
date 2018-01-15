#pragma once

#ifndef PAGEOPT_H_INCLUDED
#define PAGEOPT_H_INCLUDED

#include "common.h"

/* (ҳ��ͷ����Ϣ)
*/
struct pageMateHead{
	long pageNo;                    // ҳ��
	int curRecordNum;               // ��ǰ��ҳ�洢�ļ�¼����
	long prePageNo;					// ǰ��ҳ��
	long nextPageNo;				// ���ҳ��
	long freeSpace;                 // ��ҳ�Ŀ���ռ��С
};

/* (ҳ����) 
 * ���ڵ�ƫ������ӿ��ǰ���������
 * ���ڵļ�¼�ǴӺ���ǰ����
 */
struct OffsetInPage{
	int recordID;
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
	long allocatePage(struct DbMetaHead *head, long requestPageNum);
	void recyOnePage(struct DbMetaHead *head, long pageNo);
	void recyAllPage(struct DbMetaHead *head);
}; 

#endif // PAGEOPT_H_INCLUDED