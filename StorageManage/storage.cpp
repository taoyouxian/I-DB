#include "storage.h"


Storage::Storage()
{
}


Storage::~Storage()
{
} 

void Storage::createSysHead(char *filename){
	struct dbSysHead sysHead;
	// 配置系统参数，并写入文件中

	// 存储文件中，数据区大小，默认为196MB
	sysHead.desc.sizeOfFile = SIZE_DATA_SPACE;
	// 每页的大小，4KB
	sysHead.desc.sizePerPage = SIZE_PER_PAGE;
	// 总页数
	sysHead.desc.totalPage = (SIZE_DATA_SPACE) / (SIZE_PER_PAGE);  // 49 * 1024
	// 当前可用的页数，初始化时=totalPage
	sysHead.desc.pageAvai = sysHead.desc.totalPage;

	// bitMap 空闲空间映射表
	sysHead.desc.bitMapAddr = ADDR_BITMAP;    // bitMap在存储文件中的起始地址 =1024=1KB
	// bitMap的大小，每页用1bit，一共需要"页数"个bit，即"页数/8"个字节
	sysHead.desc.sizeBitMap = sysHead.desc.totalPage / 8;	//byte

	sysHead.desc.dataAddr = ADDR_DATA;     // 数据区起始位置，前面留给bitMap
	//sysHead.desc.nextfid = 0;
	sysHead.desc.curFileNum = 0;
	memset(sysHead.desc.fileDesc, -1, sizeof(struct FileDesc) * MAX_FILE_NUM);

	FILE *fp = fopen(filename, "wb");
	rewind(fp);
	fwrite(&(sysHead.desc), sizeof(struct SysDesc), 1, fp);// 把初始化的sysHead相关参数写到文件头部
	// 为空闲空间映射表分配空间，所有的初始化为-1，表示空闲
	sysHead.FreeSpace_bitMap = (unsigned long *)malloc(sysHead.desc.sizeBitMap);
	memset(sysHead.FreeSpace_bitMap, -1, sysHead.desc.sizeBitMap);

	// 再把bitMap写入到文件中
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
	//初始化head->desc
	rewind(fp);
	fread(&(head->desc), sizeof(struct SysDesc), 1, fp);
	//给bitMap分配空间，并初始化
	head->FreeSpace_bitMap = (unsigned long *)malloc(head->desc.sizeBitMap);
	rewind(fp);
	fseek(fp, head->desc.bitMapAddr, SEEK_SET); // 找到文件中写bitMap的位置，让fp指向这里
	fread(head->FreeSpace_bitMap, 1, head->desc.sizeBitMap, fp);// 从文件中读取bitMap的内容
	fclose(fp);

	// 初始化缓冲区
	// 将每个缓冲区块都初始化（存储的文件的页号为-1，访问时间为0，标记为未修改过）
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
	printf("文件类型:");
	if (type == NORMAL_FILE)
		printf("用户表文件\n");
	else
		printf("索引文件\n");
	printf("该文件一共占用：%ld页\n", pageNum);
	printf("该文件的起始页页号为：%ld\n", pageNo);
	printf("**************************************************\n\n");
}