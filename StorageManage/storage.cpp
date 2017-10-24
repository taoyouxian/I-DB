#include "storage.h"


Storage::Storage()
{
}


Storage::~Storage()
{
} 

void Storage::createSysHead(char *filename){
	struct dbSysHead sysHead;
	// ����ϵͳ��������д���ļ���

	// �洢�ļ��У���������С��Ĭ��Ϊ196MB
	sysHead.desc.sizeOfFile = SIZE_DATA_SPACE;
	// ÿҳ�Ĵ�С��4KB
	sysHead.desc.sizePerPage = SIZE_PER_PAGE;
	// ��ҳ��
	sysHead.desc.totalPage = (SIZE_DATA_SPACE) / (SIZE_PER_PAGE);  // 49 * 1024
	// ��ǰ���õ�ҳ������ʼ��ʱ=totalPage
	sysHead.desc.pageAvai = sysHead.desc.totalPage;

	// bitMap ���пռ�ӳ���
	sysHead.desc.bitMapAddr = ADDR_BITMAP;    // bitMap�ڴ洢�ļ��е���ʼ��ַ =1024=1KB
	// bitMap�Ĵ�С��ÿҳ��1bit��һ����Ҫ"ҳ��"��bit����"ҳ��/8"���ֽ�
	sysHead.desc.sizeBitMap = sysHead.desc.totalPage / 8;	//byte

	sysHead.desc.dataAddr = ADDR_DATA;     // ��������ʼλ�ã�ǰ������bitMap
	//sysHead.desc.nextfid = 0;
	sysHead.desc.curFileNum = 0;
	memset(sysHead.desc.fileDesc, -1, sizeof(struct FileDesc) * MAX_FILE_NUM);

	FILE *fp = fopen(filename, "wb");
	rewind(fp);
	fwrite(&(sysHead.desc), sizeof(struct SysDesc), 1, fp);// �ѳ�ʼ����sysHead��ز���д���ļ�ͷ��
	// Ϊ���пռ�ӳ������ռ䣬���еĳ�ʼ��Ϊ-1����ʾ����
	sysHead.FreeSpace_bitMap = (unsigned long *)malloc(sysHead.desc.sizeBitMap);
	memset(sysHead.FreeSpace_bitMap, -1, sysHead.desc.sizeBitMap);

	// �ٰ�bitMapд�뵽�ļ���
	rewind(fp);
	fseek(fp, sysHead.desc.bitMapAddr, SEEK_SET);
	fwrite(sysHead.FreeSpace_bitMap, 1, sysHead.desc.sizeBitMap, fp);

	fclose(fp);
}

void Storage::init_database(struct dbSysHead *head, char *filename){
	FILE *fp;
	fp = fopen(filename, "rb");
	if (fp == NULL){
		printf("Database file is not exist, begin to create database file......\n\n");
		this->createSysHead(filename);
		fp = fopen(filename, "rb");
	}
	//��ʼ��head->desc
	rewind(fp);
	fread(&(head->desc), sizeof(struct SysDesc), 1, fp);
	//��bitMap����ռ䣬����ʼ��
	head->FreeSpace_bitMap = (unsigned long *)malloc(head->desc.sizeBitMap);
	rewind(fp);
	fseek(fp, head->desc.bitMapAddr, SEEK_SET); // �ҵ��ļ���дbitMap��λ�ã���fpָ������
	fread(head->FreeSpace_bitMap, 1, head->desc.sizeBitMap, fp);// ���ļ��ж�ȡbitMap������
	fclose(fp);

	// ��ʼ��������
	// ��ÿ���������鶼��ʼ�����洢���ļ���ҳ��Ϊ-1������ʱ��Ϊ0�����Ϊδ�޸Ĺ���
	for (int i = 0; i < SIZE_BUFF; i++) {
		head->buff.map[i].pageNo = -1;
		head->buff.map[i].visitTime = 0;
		head->buff.map[i].isEdited = false;
		memset(head->buff.data[i], 0, SIZE_PER_PAGE);
	}
	head->buff.curTimeStamp = 0;
	head->fpdesc = fopen(filename, "rb+"); 
}


void Storage::show_SysDesc(struct dbSysHead *head){
	printf("**************************************************\n\n");
	printf("sizeOfFile:      %ld\n", (head->desc.sizeOfFile));
	printf("sizePerPage:     %ld\n", (head->desc.sizePerPage));
	printf("totalPage:       %ld\n", (head->desc.totalPage));
	printf("pageAvai:        %ld\n", (head->desc.pageAvai));
	printf("bitMapAddr:      %ld\n", (head->desc.bitMapAddr));
	printf("sizeBitMap:      %ld\n", (head->desc.sizeBitMap));
	printf("dataAddr:        %ld\n", (head->desc.dataAddr));
	printf("curFileNum:      %d\n", (head->desc.curFileNum));
	printf("\n**************************************************\n");
}

void Storage::show_FileDesc(struct dbSysHead *head, int fid) {
	int fx = fileOpt.queryFileIndex(head, fid);
	long pageNum = head->desc.fileDesc[fx].filePageNum;
	long pageNo = head->desc.fileDesc[fx].fileFirstPageNo;
	int type = head->desc.fileDesc[fx].fileType;
	printf("**************************************************\n\n");
	printf("�ļ�����:");
	if (type == NORMAL_FILE)
		printf("�û����ļ�\n");
	else
		printf("�����ļ�\n");
	printf("���ļ�һ��ռ�ã�%ldҳ\n", pageNum);
	printf("���ļ�����ʼҳҳ��Ϊ��%ld\n", pageNo);
	printf("**************************************************\n\n");
}