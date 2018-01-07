#pragma once

#ifndef BUFOPT_H_INCLUDED
#define BUFOPT_H_INCLUDED

#include "common.h"

/* (�������Ϣ)
*/

struct buffBlock{
	long pageNo;
	long visitTime;
	bool isEdited;
	bool isPin;
};
/* (��������Ϣ)
 * ��¼
*/ 

struct buffSpace{
	struct buffBlock map[SIZE_BUFF];
	char data[SIZE_BUFF][SIZE_PER_PAGE];
	long curTimeStamp;
};

class BufOpt
{
public:
	BufOpt();
	~BufOpt(); 

	int queryPage(struct dbSysHead *head, long queryPageNo); //��ѯҳ��query�Ƿ��ڻ������У������򷵻����ڻ������е��±�
	void replacePage(struct dbSysHead *head, int mapNo, long pageNo);// ���±�ΪmapNo�Ļ�����滻Ϊҳ��pageNo��ҳ
	int schedulingBuffer(struct dbSysHead *head);	//�����㷨���滻�����û��ʹ�õĻ������飬�����±�� (������δʹ���㷨 LRU)
	int reqPage(struct dbSysHead *head, long queryPageNo); // �����дһ��ҳ�������ڻ������У������schedulingBuffer������������
};

#endif // BUFOPT_H_INCLUDED