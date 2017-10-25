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
		printf("�����ļ�ʧ�ܣ���ǰ���ݿ��Ѵ�����%d���ļ����ﵽ���ֵ��\n", MAX_FILE_NUM);
		exit(0);
	}
	if (head->desc.pageFree < requestPageNum){
		printf("�����ļ�ʧ�ܣ���ǰ���ݿ����ҳ��Ϊ%ld���޷�������ļ������ҳ��%ld��\n", head->desc.pageFree, requestPageNum);
		exit(0);
	}
	int fileID = head->desc.curFileNum;
	head->desc.curFileNum++;
	//Ϊ�ļ�����������ҳ��alloStaPageΪ��һҳҳ��
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
		//�ҵ�segTable�е�һ�����õ��±��
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
		printf("�����ļ�ʧ�ܣ�û���㹻�������ռ䡣\n");
		exit(0);
	}
	return fileID;
}
//�����ļ���fileID����ѯ�������ݿ��е��±��
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
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fileID);
	}
	//exit(0); 
	return flag ? i : -1;
}

// ��ʱĬ��д��ļ�¼����һ��С��ҳ��С����������һ����¼��ҳ��
void FileOpt::writeFile(struct DbMetaHead *head, int fileID, int length, char *str){
	int fx = queryFileByIndex(head, fileID);
	int size_offset = sizeof(struct OffsetInPage);
	int size_pageMateHead = sizeof(struct pageMateHead);

	long PageNum = head->desc.fileInfo[0].segTable[fx].segPageNum;
	long curPageNo = head->desc.fileInfo[0].segTable[fx].segFirstPageNo;

	struct pageMateHead PHead;
	bool isFinished = false;

	for (int i = 0; i < PageNum; i++) {
		//��ҳͷ
		int mapNo = bufOpt.requestPage(head, curPageNo);
		memcpy(&PHead, head->buff.data[mapNo], size_pageMateHead);

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
			struct OffsetInPage curOffset, preOffset;
			if (PHead.curRecordNum == 0) {   //��ҳ�������κμ�¼
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

			//����ҳͷ�������µ�ƫ����oneOffset
			memcpy(head->buff.data[mapNo], &PHead, size_pageMateHead);
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

			//����ƫ���������ݣ�����ҳͷ
			int mapNo = bufOpt.requestPage(head, ph.pageNo);
			memcpy(head->buff.data[mapNo], &ph, size_pageMateHead);
			memcpy(head->buff.data[mapNo] + curOffsetPos, &curOffset, size_offset);
			memcpy(head->buff.data[mapNo] + curRecordPos, str, length);
			head->buff.map[mapNo].isEdited = true;

			//������һҳ��ҳͷ
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

		printf("--------ҳͷ��Ϣ-------\n");
		printf("ҳ�ţ�%ld\n", ph.pageNo);
		printf("ҳ�Ŀ���ռ䣺%ld\n", ph.freeSpace);
		printf("��ҳ��ǰ�洢�ļ�¼������%d\n", ph.curRecordNum);
		printf("ƫ������-----��¼����\n");
		printf("��¼��\tƫ����\t�Ƿ�ɾ��\t\n");

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