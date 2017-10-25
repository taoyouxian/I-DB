#pragma once

#ifndef BUFOPT_H_INCLUDED
#define BUFOPT_H_INCLUDED

#include "common.h"

/* (缓冲块信息)
*/
struct bufferBlock{
	long pageNo;
	long visitTime;
	bool isEdited;
};

/* (缓冲区信息)
 * 记录
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

	int queryPage(struct DbMetaHead *head, long queryPageNo); //查询页号query是否在缓冲区中，若在则返回它在缓冲区中的下标
	int replacePage(struct DbMetaHead *head, int mapNo, long pageNo);// 把下标为mapNo的缓冲块替换为页号pageNo的页
	int schedulingBuffer(struct DbMetaHead *head);	//调度算法，替换出最久没有使用的缓冲区块，返回下标号 (最近最久未使用算法 LRU)
	int requestPage(struct DbMetaHead *head, long queryPageNo); // 请求读写一个页，若不在缓冲区中，则调用schedulingBuffer调到缓冲区中
};

#endif // BUFOPT_H_INCLUDED