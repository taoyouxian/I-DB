#pragma once 

#ifndef FILEOPT_H_INCLUDED
#define FILEOPT_H_INCLUDED

#include "common.h"
#include "pageOpt.h"
#include "segmentOpt.h"
#include "bufOpt.h"


/* (文件信息) 
*/ 
struct FileInfo{
	int fileType;                // 标识是否为索引文件，以及索引文件的类型（hash/b树/顺序,线性）
	int fileID;                  // 文件号
	int fileName;                // 文件名
	int fileState;                // 文件状态
	int fileSegNum;              // 文件占用了多少段
	struct Segment segTable[SEG_NUM];		//	定义段数量
};

class FileOpt
{
public:
	FileOpt(void);
	~FileOpt(void);  
	 
	int createFile(struct DbMetaHead *head, int type, long requestPageNum);
	void readFile(struct DbMetaHead *head, int fileID, char *des);
	void writeFile(struct DbMetaHead *head, int fileID, int length, char *str);
	void deleteFile(struct DbMetaHead *head, int fileID);
	int queryFileByIndex(struct DbMetaHead *head, int fileID);

	PageOpt pageOpt;
	BufOpt bufOpt; 
}; 

#endif // PAGEOPT_H_INCLUDED