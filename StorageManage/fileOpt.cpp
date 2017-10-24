#include "storage.h"
#include "fileOpt.h"
   

FileOpt::FileOpt(void)
{
}


FileOpt::~FileOpt(void)
{
}
 
int FileOpt::createFile(struct dbSysHead *head, int type, long reqPageNum){
	if (head->desc.curFileNum >= MAX_FILE_NUM){
		printf("创建文件失败！当前数据库已创建了%d个文件，达到最大值。\n", MAX_FILE_NUM);
		exit(0);
	}
	if (head->desc.pageAvai < reqPageNum){
		printf("创建文件失败！当前数据库可用页数为%ld，无法满足该文件请求的页数%ld。\n", head->desc.pageAvai, reqPageNum);
		exit(0);
	}
	int fid = head->desc.curFileNum;
	head->desc.curFileNum++;
	//为文件分配连续的页，alloStaPage为第一页页号
	long alloStaPage = pageOpt.allocatePage(head, reqPageNum);
	if (alloStaPage >= 0){
		for (long i = 0; i < reqPageNum; i++) {
			struct pageHead ph;
			ph.pageNo = alloStaPage + i;
			ph.curRecordNum = 0;
			if (i == 0) {
				ph.prePageNo = -1;
				if (reqPageNum == 1)
					ph.nextPageNo = -1;
				else
					ph.nextPageNo = ph.pageNo + 1;
			}
			else {
				ph.prePageNo = ph.pageNo - 1;
				if (i == reqPageNum - 1)
					ph.nextPageNo = -1;
				else
					ph.nextPageNo = ph.pageNo + 1;
			}
			ph.freeSpace = SIZE_PER_PAGE - sizeof(struct pageHead);

			rewind(head->fpdesc);
			fseek(head->fpdesc, head->desc.dataAddr + ph.pageNo * SIZE_PER_PAGE, SEEK_SET);
			fwrite(&ph, sizeof(ph), 1, head->fpdesc);
		}
		//找到fileDesc中第一个可用的下标号
		int i;
		for (i = 0; i < MAX_FILE_NUM; i++) {
			if ((head->desc).fileDesc[i].fileID <= 0) {
				break;
			}
		}
		head->desc.fileDesc[i].fileType = type;
		head->desc.fileDesc[i].fileID = fid;
		head->desc.fileDesc[i].fileFirstPageNo = alloStaPage;
		head->desc.fileDesc[i].filePageNum = reqPageNum;
	}
	else {
		printf("创建文件失败！没有足够的连续空间。\n");
		exit(0);
	}
	return fid;
}
//给定文件号fid，查询其在数据库中的下标号
int FileOpt::queryFileIndex(struct dbSysHead *head, int fid){
	bool flag = false;
	int i;
	for (i = 0; i < MAX_FILE_NUM; i++) {
		if (head->desc.fileDesc[i].fileID == fid){
			flag = true;
			break;
		}
	}
	if (!flag){
		printf("数据库中不存在文件号为%d的文件！\n", fid);
	}
	//exit(0); 
	return flag ? i : -1;
}

// 暂时默认写入的记录长度一定小于页大小（即不允许一条记录跨页）
void FileOpt::writeFile(struct dbSysHead *head, int fid, int length, char *str){
	int fx = queryFileIndex(head, fid);
	int size_offset = sizeof(struct offsetInPage);
	int size_pagehead = sizeof(struct pageHead);

	long PageNum = head->desc.fileDesc[fx].filePageNum;
	long curPageNo = head->desc.fileDesc[fx].fileFirstPageNo;

	struct pageHead PHead;
	bool isFinished = false;

	for (int i = 0; i < PageNum; i++) {
		//读页头
		int mapNo = bufOpt.reqPage(head, curPageNo);
		memcpy(&PHead, head->buff.data[mapNo], size_pagehead);

		//本页的空余空间不够加一条数据和一个偏移量了，找下一页
		if (PHead.freeSpace - size_offset <= length){
			long nextpid = PHead.nextPageNo;
			if (nextpid == -1)
				break;
			else {
				curPageNo = nextpid;
				continue;
			}
		}
		else {
			long curRecordPos, curOffsetPos;
			struct offsetInPage curOffset, preOffset;
			if (PHead.curRecordNum == 0) {   //该页中暂无任何记录
				curOffset.recordID = 0;
				curOffset.offset = length;
				curOffset.isDeleted = false;
				curRecordPos = SIZE_PER_PAGE - length;
				curOffsetPos = size_pagehead;
			}
			else {
				int curRecordID = PHead.curRecordNum;

				memcpy(&preOffset, head->buff.data[mapNo] + size_pagehead + size_offset * (PHead.curRecordNum - 1), size_offset);
				curOffset.recordID = curRecordID;
				curOffset.offset = preOffset.offset + length;
				curOffset.isDeleted = false;
				curRecordPos = SIZE_PER_PAGE - preOffset.offset - length;
				curOffsetPos = size_pagehead + size_offset * curRecordID;
			}
			PHead.curRecordNum++;
			PHead.freeSpace -= length;
			PHead.freeSpace -= size_offset;

			//更新页头，插入新的偏移量oneOffset
			memcpy(head->buff.data[mapNo], &PHead, size_pagehead);
			memcpy(head->buff.data[mapNo] + curOffsetPos, &curOffset, size_offset);
			memcpy(head->buff.data[mapNo] + curRecordPos, str, length);
			head->buff.map[mapNo].isEdited = true;
			isFinished = true;
			break;
		}
	}
	//该文件中没有哪个页有足够的空间可以写下该记录，扩展文件，再分配一个新页
	if (isFinished == false) {
		long newPageID = pageOpt.allocatePage(head, 1);
		if (newPageID >= 0) {
			head->desc.fileDesc[fid].filePageNum++;
			struct pageHead ph;
			ph.pageNo = newPageID;
			ph.prePageNo = PHead.pageNo;
			ph.nextPageNo = -1;
			PHead.nextPageNo = newPageID;

			struct offsetInPage curOffset;
			curOffset.recordID = 0;
			curOffset.offset = length;
			curOffset.isDeleted = false;

			long curRecordPos = SIZE_PER_PAGE - length;
			long curOffsetPos = size_pagehead;

			ph.curRecordNum = 1;
			ph.freeSpace = SIZE_PER_PAGE - size_pagehead - size_offset - length;

			//插入偏移量和数据，更新页头
			int mapNo = bufOpt.reqPage(head, ph.pageNo);
			memcpy(head->buff.data[mapNo], &ph, size_pagehead);
			memcpy(head->buff.data[mapNo] + curOffsetPos, &curOffset, size_offset);
			memcpy(head->buff.data[mapNo] + curRecordPos, str, length);
			head->buff.map[mapNo].isEdited = true;

			//更新上一页的页头
			mapNo = bufOpt.reqPage(head, PHead.pageNo);
			memcpy(head->buff.data[mapNo], &PHead, size_pagehead);
			head->buff.map[mapNo].isEdited = true;
		}
	}

}

void FileOpt::readFile(struct dbSysHead *head, int fid, char *des){
	int fx = queryFileIndex(head, fid);
	long pageNum = head->desc.fileDesc[fx].filePageNum;
	long pageNo = head->desc.fileDesc[fx].fileFirstPageNo;
	struct pageHead ph;
	struct offsetInPage curOffset, preOffset;
	int mapNo = -1;

	int size_offset = sizeof(struct offsetInPage);
	int size_pagehead = sizeof(struct pageHead);

	for (int i = 0; i < pageNum; i++){
		mapNo = bufOpt.reqPage(head, pageNo);
		memcpy(&ph, head->buff.data[mapNo], size_pagehead);

		printf("--------页头信息-------\n");
		printf("页号：%ld\n", ph.pageNo);
		printf("页的空余空间：%ld\n", ph.freeSpace);
		printf("该页当前存储的记录个数：%d\n", ph.curRecordNum);
		printf("偏移量表-----记录内容\n");
		printf("记录号\t偏移量\t是否删除\t\n");

		memcpy(&preOffset, head->buff.data[mapNo] + size_pagehead, size_offset);
		int readLength = 0;

		for (int j = 0; j < ph.curRecordNum; j++) {
			memcpy(&curOffset, head->buff.data[mapNo] + size_pagehead + size_offset * j, size_offset);
			printf("%d\t\t%d\t\t%d\t", curOffset.recordID, curOffset.offset, curOffset.isDeleted);

			if (j == 0)
				readLength = curOffset.offset;
			else
				readLength = curOffset.offset - preOffset.offset;

			memcpy(des, head->buff.data[mapNo] + SIZE_PER_PAGE - curOffset.offset, readLength);
			des[readLength] = '\0';
			printf("%s\n", des);

			preOffset = curOffset;
		}
		long nextpid = ph.nextPageNo;
		if (nextpid < 0)
			break;
		else
			pageNo = nextpid;
	}
}

void FileOpt::deleteFile(struct dbSysHead *head, int fid){
	int fx = queryFileIndex(head, fid);
	if (fx >= 0) {
		long pageNum = head->desc.fileDesc[fx].filePageNum;
		long pageNo = head->desc.fileDesc[fx].fileFirstPageNo;
		long pageAddr = head->desc.dataAddr + pageNo * SIZE_PER_PAGE;
		long nextPage = -1;

		for (long i = 0; i < pageNum; i++) {
			struct pageHead ph;
			rewind(head->fpdesc);
			fseek(head->fpdesc, pageAddr, SEEK_SET);
			fread(&ph, sizeof(struct pageHead), 1, head->fpdesc);

			nextPage = ph.nextPageNo;
			pageOpt.recyOnePage(head, ph.pageNo);

			if (nextPage < 0)
				break;
			else
				pageAddr = head->desc.dataAddr + nextPage * SIZE_PER_PAGE;
		}

		head->desc.pageAvai += pageNum;
		head->desc.curFileNum--;
		head->desc.fileDesc[fx].fileType = -1;
		head->desc.fileDesc[fx].fileID = -1;
		head->desc.fileDesc[fx].fileFirstPageNo = -1;
		head->desc.fileDesc[fx].filePageNum = -1;
	}
}