#pragma once 

#ifndef FILEOPT_H_INCLUDED
#define FILEOPT_H_INCLUDED

#include "common.h"
#include "pageOpt.h"
#include "bufOpt.h"

class FileOpt
{
public:
	FileOpt(void);
	~FileOpt(void);  

	//fileOption.cpp
	int createFile(struct dbSysHead *head, int type, long reqPageNum);
	int queryFileIndex(struct dbSysHead *head, int fid);
	void writeFile(struct dbSysHead *head, int fid, int length, char *str);
	void readFile(struct dbSysHead *head, int fid, char *des);
	void deleteFile(struct dbSysHead *head, int fid);

	PageOpt pageOpt;
	BufOpt bufOpt;
	 

}; 

#endif // PAGEOPT_H_INCLUDED