#pragma once

#ifndef BUFOPT_H_INCLUDED
#define BUFOPT_H_INCLUDED

#include "common.h"

class BufOpt
{
public:
	BufOpt();
	~BufOpt(); 

	//buffManage.cpp
	int queryPage(struct dbSysHead *head, long queryPageNo); //��ѯҳ��query�Ƿ��ڻ������У������򷵻����ڻ������е��±�
	int replacePage(struct dbSysHead *head, int mapNo, long pageNo);// ���±�ΪmapNo�Ļ�����滻Ϊҳ��pageNo��ҳ
	int scheBuff(struct dbSysHead *head);                 //�����㷨���滻�����û��ʹ�õ�ҳ
	int reqPage(struct dbSysHead *head, long queryPageNo); // �����дһ��ҳ�������ڻ������У������scheBuff������������
};

#endif // BUFOPT_H_INCLUDED