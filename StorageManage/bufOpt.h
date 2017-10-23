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
	int queryPage(struct dbSysHead *head, long queryPageNo); //查询页号query是否在缓冲区中，若在则返回它在缓冲区中的下标
	int replacePage(struct dbSysHead *head, int mapNo, long pageNo);// 把下标为mapNo的缓冲块替换为页号pageNo的页
	int scheBuff(struct dbSysHead *head);                 //调度算法，替换出最久没有使用的页
	int reqPage(struct dbSysHead *head, long queryPageNo); // 请求读写一个页，若不在缓冲区中，则调用scheBuff调到缓冲区中
};

#endif // BUFOPT_H_INCLUDED