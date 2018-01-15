#pragma once

#ifndef BUFOPT_H_INCLUDED
#define BUFOPT_H_INCLUDED

#include "common.h"

/* (�������Ϣ)
*/
struct bufferBlock{
	long pageNo;
	long visitTime;
	bool isEdited;
};

/* (��������Ϣ)
 * ��¼
*/
struct bufferPool{
	struct bufferBlock map[BUFFER_Size];
	char data[BUFFER_Size][PAGE_SIZE];
	long curTimeStamp;
};

class BufOpt
{
public:
	BufOpt();
	~BufOpt(); 

	int queryPage(struct DbMetaHead *head, long queryPageNo); //��ѯҳ��query�Ƿ��ڻ������У������򷵻����ڻ������е��±�
	int replacePage(struct DbMetaHead *head, int mapNo, long pageNo);// ���±�ΪmapNo�Ļ�����滻Ϊҳ��pageNo��ҳ
	int schedulingBuffer(struct DbMetaHead *head);	//�����㷨���滻�����û��ʹ�õĻ������飬�����±�� (������δʹ���㷨 LRU)
	int requestPage(struct DbMetaHead *head, long queryPageNo); // �����дһ��ҳ�������ڻ������У������schedulingBuffer������������
};

#endif // BUFOPT_H_INCLUDED