#pragma once

#ifndef STORAGE_H_INCLUDED
#define STORAGE_H_INCLUDED

#include "ui_glwidget.h"
#include "common.h"
#include "fileOpt.h"
#include "bufOpt.h"
#include "pageOpt.h"
#include "segment.h"
#include "Test.h"
 
class MainWindow;

struct DbInfo{ 
	struct FileInfo fileInfo[MAX_FILE_NUM];	    //	对每一个文件进行描述

	long fileSize;                            //	文件中数据区的大小，默认设为196MB
	long pageSize;                           //	每一个页的大小，默认4KB
	long pageSum;                             //	总共的页数
	long pageFree;                              //	当前有多少可用的页  

	//bitMap 空闲空间位示图
	long bitMapAddStart;                            //	文件中bitMap的起始地址
	long bitMapSize;                            //	bitMap的大小，以字节为单位

	long dataAddStart;                              //  文件中数据区的起始位置
	int curFileNum;                            //	目前有多少个文件，最多为 MAX_FILE_NUM 

	long MapTable_firstPageNo;              //映射表的起始页号
	long curAvaiLogicID;                    //当前第一个可用的逻辑号

};

struct DbMetaHead{
	struct DbInfo desc;
	unsigned long *FreeSpace_bitMap;
	struct bufferPool buff;
	FILE *dataPath;
};

//更新，把内存中的所有数据写入到磁盘中
int memToDisk(struct DbMetaHead *head);

class Storage
{
private:
	vector<std::vector<supplier*>>		m_suppliers;

public:
	Storage();
	~Storage();
	 
	void createDbMetaHead(char *fileName); 
	void initDB(struct DbMetaHead *head, char *fileName, MainWindow *ui);
	void showDbInfo(struct DbMetaHead *head);
	void showSegTable(struct DbMetaHead *head, int fileID); 

	FileOpt fileOpt;
	BufOpt bufOpt;
	PageOpt pageOpt;
};

#endif // STORAGE_H_INCLUDED
