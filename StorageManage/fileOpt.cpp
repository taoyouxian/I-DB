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
		printf("�����ļ�ʧ�ܣ���ǰ���ݿ��Ѵ�����%d���ļ����ﵽ���ֵ��\n", MAX_FILE_NUM);
		exit(0);
	}
	if (head->desc.pageAvai < reqPageNum){
		printf("�����ļ�ʧ�ܣ���ǰ���ݿ����ҳ��Ϊ%ld���޷�������ļ������ҳ��%ld��\n", head->desc.pageAvai, reqPageNum);
		exit(0);
	}
	int fid = head->desc.curFileNum;
	head->desc.curFileNum++;
	//Ϊ�ļ�����������ҳ��alloStaPageΪ��һҳҳ��
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
		//�ҵ�fileDesc�е�һ�����õ��±��
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
		printf("�����ļ�ʧ�ܣ�û���㹻�������ռ䡣\n");
		exit(0);
	}
	return fid;
}
//�����ļ���fid����ѯ�������ݿ��е��±��
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
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
	}
	//exit(0); 
	return flag ? i : -1;
}

// ��ʱĬ��д��ļ�¼����һ��С��ҳ��С����������һ����¼��ҳ��
void FileOpt::writeFile(struct dbSysHead *head, int fid, int length, char *str){
	int fx = queryFileIndex(head, fid);
	int size_offset = sizeof(struct offsetInPage);
	int size_pagehead = sizeof(struct pageHead);

	long PageNum = head->desc.fileDesc[fx].filePageNum;
	long curPageNo = head->desc.fileDesc[fx].fileFirstPageNo;

	struct pageHead PHead;
	bool isFinished = false;

	for (int i = 0; i < PageNum; i++) {
		//��ҳͷ
		int mapNo = bufOpt.reqPage(head, curPageNo);
		memcpy(&PHead, head->buff.data[mapNo], size_pagehead);

		//��ҳ�Ŀ���ռ䲻����һ�����ݺ�һ��ƫ�����ˣ�����һҳ
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
			if (PHead.curRecordNum == 0) {   //��ҳ�������κμ�¼
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

			//����ҳͷ�������µ�ƫ����oneOffset
			memcpy(head->buff.data[mapNo], &PHead, size_pagehead);
			memcpy(head->buff.data[mapNo] + curOffsetPos, &curOffset, size_offset);
			memcpy(head->buff.data[mapNo] + curRecordPos, str, length);
			head->buff.map[mapNo].isEdited = true;
			isFinished = true;
			break;
		}
	}
	//���ļ���û���ĸ�ҳ���㹻�Ŀռ����д�¸ü�¼����չ�ļ����ٷ���һ����ҳ
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

			//����ƫ���������ݣ�����ҳͷ
			int mapNo = bufOpt.reqPage(head, ph.pageNo);
			memcpy(head->buff.data[mapNo], &ph, size_pagehead);
			memcpy(head->buff.data[mapNo] + curOffsetPos, &curOffset, size_offset);
			memcpy(head->buff.data[mapNo] + curRecordPos, str, length);
			head->buff.map[mapNo].isEdited = true;

			//������һҳ��ҳͷ
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

		printf("--------ҳͷ��Ϣ-------\n");
		printf("ҳ�ţ�%ld\n", ph.pageNo);
		printf("ҳ�Ŀ���ռ䣺%ld\n", ph.freeSpace);
		printf("��ҳ��ǰ�洢�ļ�¼������%d\n", ph.curRecordNum);
		printf("ƫ������-----��¼����\n");
		printf("��¼��\tƫ����\t�Ƿ�ɾ��\t\n");

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