#pragma once

#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED

#include "common.h"
#include "fileOpt.h"
#include "bufOpt.h"
#include "pageOpt.h"

#define MAX_FILE_NUM 64

#define SIZE_BUFF 1000
#define SIZE_PER_PAGE (4*1024)
#define SIZE_DATA_SPACE (196*1024*1024)
#define SIZE_BIT_MAP  (SIZE_DATA_SPACE/(SIZE_PER_PAGE*8))

#define ADDR_BITMAP 1024
#define ADDR_DATA (ADDR_BITMAP+SIZE_BIT_MAP)

#define MAP_FILE         0
#define NORMAL_FILE      1
#define INDEX_FILE_HASH  2
#define INDEX_FILE_BTREE 3
#define INDEX_FILE_SEQ   4

#define ALLO_FAIL  -1
#define PAGE_AVAI   1
#define PAGE_UNAVAI 0

#define BUFF_NOT_HIT -1


struct buffBlock{
	long pageNo;
	long visitTime;
	bool isEdited;
};

struct buffSpace{
	struct buffBlock map[SIZE_BUFF];
	char data[SIZE_BUFF][SIZE_PER_PAGE];
	long curTimeStamp;
};

/* 块内的偏移量表从块的前端向后增长
* 块内的记录是从后向前放置
*/
struct offsetInPage{
	int recordID;
	int offset;         //该记录相对于块尾地址的偏移量
	bool isDeleted;
};

struct pageHead{
	long pageNo;                    // 页号
	int curRecordNum;               // 当前该页存储的记录个数
	long prePageNo;
	long nextPageNo;
	long freeSpace;                 // 该页的空余空间大小
};

struct FileDesc{
	int fileType;               //标识是否为索引文件，以及索引文件的类型（hash/b树/顺序）
	int fileID;                    //文件号
	long fileFirstPageNo;
	long filePageNum;              //文件占用了多少页
};

struct SysDesc{
	long sizeOfFile;                            //	文件中数据区的大小，默认设为196MB
	long sizePerPage;                           //	每一个页的大小，默认4KB
	long totalPage;                             //	总共的页数
	long pageAvai;                              //	当前有多少可用的页

	//bitMap 空闲空间位示图
	long bitMapAddr;                            //	文件中bitMap的起始地址
	long sizeBitMap;                            //	bitMap的大小，以字节为单位

	long dataAddr;                              //  文件中数据区的起始位置
	int curFileNum;                            //	目前有多少个文件，最多为 MAX_FILE_NUM
	struct FileDesc fileDesc[MAX_FILE_NUM];		//	对每一个文件进行描述

	long MapTable_firstPageNo;              //映射表的起始页号
	long curAvaiLogicID;                    //当前第一个可用的逻辑号

};

struct dbSysHead{
	struct SysDesc desc;
	unsigned long *FreeSpace_bitMap;
	struct buffSpace buff;
	FILE *fpdesc;
};

class Storage
{
public:
	Storage();
	~Storage();

	//initial.cpp
	void createSysHead(char *filename); 
	void init_database(struct dbSysHead *head, char *filename);
	void show_SysDesc(struct dbSysHead *head);
	void show_FileDesc(struct dbSysHead *head, int fid); 

	FileOpt fileOpt;
	BufOpt bufOpt;
	PageOpt pageOpt;
};

#endif // STORAGE_H_INCLUDED
