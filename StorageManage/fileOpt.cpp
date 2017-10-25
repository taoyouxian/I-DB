#include "storage.h"
#include "fileOpt.h"
   

FileOpt::FileOpt(void)
{
}


FileOpt::~FileOpt(void)
{
}
 
int FileOpt::createFile(struct DbMetaHead *head, int type, long requestPageNum){
	if (head->desc.curFileNum >= MAX_FILE_NUM){
		printf("创建文件失败！当前数据库已创建了%d个文件，达到最大值。\n", MAX_FILE_NUM);
		exit(0);
	}
	if (head->desc.pageFree < requestPageNum){
		printf("创建文件失败！当前数据库可用页数为%ld，无法满足该文件请求的页数%ld。\n", head->desc.pageFree, requestPageNum);
		exit(0);
	}
	int fileID = head->desc.curFileNum;
	head->desc.curFileNum++;
	//为文件分配连续的页，alloStaPage为第一页页号
	long alloStaPage = pageOpt.allocatePage(head, requestPageNum);
	if (alloStaPage >= 0){
		for (long i = 0; i < requestPageNum; i++) {
			struct pageMateHead ph;
			ph.pageNo = alloStaPage + i;
			ph.curRecordNum = 0;
			if (i == 0) {
				ph.prePageNo = -1;
				if (requestPageNum == 1)
					ph.nextPageNo = -1;
				else
					ph.nextPageNo = ph.pageNo + 1;
			}
			else {
				ph.prePageNo = ph.pageNo - 1;
				if (i == requestPageNum - 1)
					ph.nextPageNo = -1;
				else
					ph.nextPageNo = ph.pageNo + 1;
			}
			ph.freeSpace = PAGE_SIZE - sizeof(struct pageMateHead);

			rewind(head->dataPath);
			fseek(head->dataPath, head->desc.dataAddStart + ph.pageNo * PAGE_SIZE, SEEK_SET);
			fwrite(&ph, sizeof(ph), 1, head->dataPath);
		}
		//找到segTable中第一个可用的下标号
		int i;
		for (i = 0; i < MAX_FILE_NUM; i++) {
			if ((head->desc).fileInfo[0].segTable[i].segID <= 0) {
				break;
			}
		}
		head->desc.fileInfo[0].segTable[i].segType = type;
		head->desc.fileInfo[0].segTable[i].segID = fileID;
		head->desc.fileInfo[0].segTable[i].segFirstPageNo = alloStaPage;
		head->desc.fileInfo[0].segTable[i].segPageNum = requestPageNum;
	}
	else {
		printf("创建文件失败！没有足够的连续空间。\n");
		exit(0);
	}
	return fileID;
}
//给定文件号fileID，查询其在数据库中的下标号
int FileOpt::queryFileByIndex(struct DbMetaHead *head, int fileID){
	bool flag = false;
	int i;
	for (i = 0; i < MAX_FILE_NUM; i++) {
		if (head->desc.fileInfo[0].segTable[i].segID == fileID){
			flag = true;
			break;
		}
	}
	if (!flag){
		printf("数据库中不存在文件号为%d的文件！\n", fileID);
	}
	//exit(0); 
	return flag ? i : -1;
}

// 暂时默认写入的记录长度一定小于页大小（即不允许一条记录跨页）
void FileOpt::writeFile(struct DbMetaHead *head, int fileID, int length, char *str){
	int fx = queryFileByIndex(head, fileID);
	int size_offset = sizeof(struct OffsetInPage);
	int size_pageMateHead = sizeof(struct pageMateHead);

	long PageNum = head->desc.fileInfo[0].segTable[fx].segPageNum;
	long curPageNo = head->desc.fileInfo[0].segTable[fx].segFirstPageNo;

	struct pageMateHead PHead;
	bool isFinished = false;

	for (int i = 0; i < PageNum; i++) {
		//读页头
		int mapNo = bufOpt.requestPage(head, curPageNo);
		memcpy(&PHead, head->buff.data[mapNo], size_pageMateHead);

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
			struct OffsetInPage curOffset, preOffset;
			if (PHead.curRecordNum == 0) {   //该页中暂无任何记录
				curOffset.recordID = 0;
				curOffset.offset = length;
				curOffset.isDeleted = false;
				curRecordPos = PAGE_SIZE - length;
				curOffsetPos = size_pageMateHead;
			}
			else {
				int curRecordID = PHead.curRecordNum;

				memcpy(&preOffset, head->buff.data[mapNo] + size_pageMateHead + size_offset * (PHead.curRecordNum - 1), size_offset);
				curOffset.recordID = curRecordID;
				curOffset.offset = preOffset.offset + length;
				curOffset.isDeleted = false;
				curRecordPos = PAGE_SIZE - preOffset.offset - length;
				curOffsetPos = size_pageMateHead + size_offset * curRecordID;
			}
			PHead.curRecordNum++;
			PHead.freeSpace -= length;
			PHead.freeSpace -= size_offset;

			//更新页头，插入新的偏移量oneOffset
			memcpy(head->buff.data[mapNo], &PHead, size_pageMateHead);
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
			head->desc.fileInfo[0].segTable[fileID].segPageNum++;
			struct pageMateHead ph;
			ph.pageNo = newPageID;
			ph.prePageNo = PHead.pageNo;
			ph.nextPageNo = -1;
			PHead.nextPageNo = newPageID;

			struct OffsetInPage curOffset;
			curOffset.recordID = 0;
			curOffset.offset = length;
			curOffset.isDeleted = false;

			long curRecordPos = PAGE_SIZE - length;
			long curOffsetPos = size_pageMateHead;

			ph.curRecordNum = 1;
			ph.freeSpace = PAGE_SIZE - size_pageMateHead - size_offset - length;

			//插入偏移量和数据，更新页头
			int mapNo = bufOpt.requestPage(head, ph.pageNo);
			memcpy(head->buff.data[mapNo], &ph, size_pageMateHead);
			memcpy(head->buff.data[mapNo] + curOffsetPos, &curOffset, size_offset);
			memcpy(head->buff.data[mapNo] + curRecordPos, str, length);
			head->buff.map[mapNo].isEdited = true;

			//更新上一页的页头
			mapNo = bufOpt.requestPage(head, PHead.pageNo);
			memcpy(head->buff.data[mapNo], &PHead, size_pageMateHead);
			head->buff.map[mapNo].isEdited = true;
		}
	}

}

void FileOpt::readFile(struct DbMetaHead *head, int fileID, char *des){
	int fx = queryFileByIndex(head, fileID);
	long pageNum = head->desc.fileInfo[0].segTable[fx].segPageNum;
	long pageNo = head->desc.fileInfo[0].segTable[fx].segFirstPageNo;
	struct pageMateHead ph;
	struct OffsetInPage curOffset, preOffset;
	int mapNo = -1;

	int size_offset = sizeof(struct OffsetInPage);
	int size_pageMateHead = sizeof(struct pageMateHead);

	for (int i = 0; i < pageNum; i++){
		mapNo = bufOpt.requestPage(head, pageNo);
		memcpy(&ph, head->buff.data[mapNo], size_pageMateHead);

		printf("--------页头信息-------\n");
		printf("页号：%ld\n", ph.pageNo);
		printf("页的空余空间：%ld\n", ph.freeSpace);
		printf("该页当前存储的记录个数：%d\n", ph.curRecordNum);
		printf("偏移量表-----记录内容\n");
		printf("记录号\t偏移量\t是否删除\t\n");

		memcpy(&preOffset, head->buff.data[mapNo] + size_pageMateHead, size_offset);
		int readLength = 0;

		for (int j = 0; j < ph.curRecordNum; j++) {
			memcpy(&curOffset, head->buff.data[mapNo] + size_pageMateHead + size_offset * j, size_offset);
			printf("%d\t\t%d\t\t%d\t", curOffset.recordID, curOffset.offset, curOffset.isDeleted);

			if (j == 0)
				readLength = curOffset.offset;
			else
				readLength = curOffset.offset - preOffset.offset;

			memcpy(des, head->buff.data[mapNo] + PAGE_SIZE - curOffset.offset, readLength);
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

void FileOpt::deleteFile(struct DbMetaHead *head, int fileID){
	int fx = queryFileByIndex(head, fileID);
	if (fx >= 0) {
		long pageNum = head->desc.fileInfo[0].segTable[fx].segPageNum;
		long pageNo = head->desc.fileInfo[0].segTable[fx].segFirstPageNo;
		long pageAddr = head->desc.dataAddStart + pageNo * PAGE_SIZE;
		long nextPage = -1;

		for (long i = 0; i < pageNum; i++) {
			struct pageMateHead ph;
			rewind(head->dataPath);
			fseek(head->dataPath, pageAddr, SEEK_SET);
			fread(&ph, sizeof(struct pageMateHead), 1, head->dataPath);

			nextPage = ph.nextPageNo;
			pageOpt.recyOnePage(head, ph.pageNo);

			if (nextPage < 0)
				break;
			else
				pageAddr = head->desc.dataAddStart + nextPage * PAGE_SIZE;
		}

		head->desc.pageFree += pageNum;
		head->desc.curFileNum--;
		head->desc.fileInfo[0].segTable[fx].segType = -1;
		head->desc.fileInfo[0].segTable[fx].segID = -1;
		head->desc.fileInfo[0].segTable[fx].segFirstPageNo = -1;
		head->desc.fileInfo[0].segTable[fx].segPageNum = -1;
	}
}