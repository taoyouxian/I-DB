#pragma once

#ifndef PAGEOPT_H_INCLUDED
#define PAGEOPT_H_INCLUDED

#include "common.h"

class PageOpt
{
public:
	PageOpt();
	~PageOpt();

	//pageOption.cpp
	int getBit(unsigned long num, long pos);
	int setBit(unsigned long *num, long pos, int setValue);
	long allocatePage(struct dbSysHead *head, long reqPageNum);
	void recyOnePage(struct dbSysHead *head, long pageNo);
	void recyAllPage(struct dbSysHead *head);
};


#endif // PAGEOPT_H_INCLUDED