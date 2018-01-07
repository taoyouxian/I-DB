#pragma once

#ifndef PAGEOPT_H_INCLUDED
#define PAGEOPT_H_INCLUDED

#include "common.h"

/* (页表头部信息)
*/ 
struct pageHead{
	long pageNo;                    // 页号
	int curRecordNum;               // 当前该页存储的记录个数
	long prePageNo;
	long nextPageNo;
	long freeSpace;                 // 该页的空余空间大小
};

/* (页表中) 
 * 块内的偏移量表从块的前端向后增长
 * 块内的记录是从后向前放置
 */ 
struct offsetInPage{
	long logicID;       // 该记录在db中的逻辑号（作为逻辑地址）
	int recordID;       // 该记录在当前页的记录号
	int offset;         //该记录相对于块尾地址的偏移量
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