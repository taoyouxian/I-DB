#include "dbHead.h"

void creatSysHead(char *filename){
	struct dbSysHead sysHead; // 配置系统参数，并写入文件中

	// 存储文件中，数据区大小，默认为196MB
	sysHead.desc.sizeOfFile = SIZE_DATA_SPACE;
	// 每页的大小，4KB
	sysHead.desc.sizePerPage = SIZE_PER_PAGE;
	// 总页数
	sysHead.desc.TotalPage = (SIZE_DATA_SPACE) / (SIZE_PER_PAGE);
	// 当前可用的页数，初始化时=totalPage
	sysHead.desc.AvaiPage = sysHead.desc.TotalPage;

	// 空闲空间位示图的起始地址
	sysHead.desc.addr_FreeSpace_BitMap = ADDR_FREE_SPACE_BIT_MAP;
	// 大小，每页用1bit，一共需要"页数"个bit，即"页数/8"个字节
	sysHead.desc.size_FreeSpace_BitMap = sysHead.desc.TotalPage / 8;	//byte

	// 空闲逻辑号位示图的起始地址
	sysHead.desc.addr_FreeLogicID_BitMap = ADDR_FREE_LOGICID_BIT_MAP;
	// 其大小，暂时静态分配1024000个可用逻辑号
	sysHead.desc.size_FreeLogicID_BitMap = SIZE_FREE_LOGICID_BIT_MAP;

	sysHead.desc.addr_data = ADDR_DATA;     //数据区起始位置，前面留给两个bitMap
	sysHead.desc.fid_MapTable = -1;
	sysHead.desc.fid_DataDictionary = -1;

	sysHead.desc.curRecordNum = 0;                      //当前已有的记录总数
	sysHead.desc.curFileNum = 0;
	memset(sysHead.desc.fileDesc, -1, sizeof(struct FileDesc) * MAX_FILE_NUM);


	FILE *fp = fopen(filename, "wb");
	rewind(fp);
	fwrite(&(sysHead.desc), sizeof(struct SysDesc), 1, fp);// 把初始化的sysHead相关参数写到文件头部
	// 为空闲空间映射表分配空间，所有的初始化为-1，表示空闲
	sysHead.FreeSpace_bitMap = (unsigned long *)malloc(sysHead.desc.size_FreeSpace_BitMap);
	memset(sysHead.FreeSpace_bitMap, -1, sysHead.desc.size_FreeSpace_BitMap);

	sysHead.FreeLogicID_bitMap = (unsigned long *)malloc(sysHead.desc.size_FreeLogicID_BitMap);
	memset(sysHead.FreeLogicID_bitMap, -1, sysHead.desc.size_FreeLogicID_BitMap);

	// 再写入到文件中
	rewind(fp);
	fseek(fp, sysHead.desc.addr_FreeSpace_BitMap, SEEK_SET);
	fwrite(sysHead.FreeSpace_bitMap, 1, sysHead.desc.size_FreeSpace_BitMap, fp);

	fseek(fp, sysHead.desc.addr_FreeLogicID_BitMap, SEEK_SET);
	fwrite(sysHead.FreeLogicID_bitMap, 1, sysHead.desc.size_FreeLogicID_BitMap, fp);

	fclose(fp);
}

void init_database(struct dbSysHead *head, char *filename){
	printf("\nInit database...\n");
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL){
		printf("database not exist, start to create database...\n");
		creatSysHead(filename);
		fp = fopen(filename, "rb");
	}
	//初始化head->desc
	rewind(fp);
	fread(&(head->desc), sizeof(struct SysDesc), 1, fp);
	//分配空间，并初始化
	head->FreeSpace_bitMap = (unsigned long *)malloc(head->desc.size_FreeSpace_BitMap);
	rewind(fp);
	fseek(fp, head->desc.addr_FreeSpace_BitMap, SEEK_SET); // 找到文件中写bitMap的位置，让fp指向这里
	fread(head->FreeSpace_bitMap, 1, head->desc.size_FreeSpace_BitMap, fp);// 从文件中读取bitMap的内容

	head->FreeLogicID_bitMap = (unsigned long*)malloc(head->desc.size_FreeLogicID_BitMap);
	fseek(fp, head->desc.addr_FreeLogicID_BitMap, SEEK_SET);
	fread(head->FreeLogicID_bitMap, 1, head->desc.size_FreeLogicID_BitMap, fp);

	fclose(fp);
	head->fpdesc = fopen(filename, "rb+");

	// 初始化缓冲区
	// 将每个缓冲区块都初始化（存储的文件的页号为-1，访问时间为0，标记为未修改过）
	for (int i = 0; i < SIZE_BUFF; i++) {
		head->buff.map[i].pageNo = -1;
		head->buff.map[i].visitTime = 0;
		head->buff.map[i].isEdited = false;
		head->buff.map[i].isPin = false;
		memset(head->buff.data[i], 0, SIZE_PER_PAGE);
	}
	head->buff.curTimeStamp = 0;

	int count = 0;
	//从数据字典文件中读取关系模式，再初始化data_dict
	if (head->desc.fid_DataDictionary >= 0){
		count = readDataDictionaryFile(head);
	}

	for (int i = count; i < MAX_FILE_NUM; i++) {
		memset(&head->data_dict[i], 0, sizeof(Relation));
		head->data_dict[i].fileID = -1;
	}
	printf("Init database successfully.\n");
}

int readDataDictionaryFile(struct dbSysHead *head) {
	int fid = head->desc.fid_DataDictionary;
	if (fid < 0) {
		printf("Data dictionary file does not exist.");
		return 0;
	}
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	int count = 0;
	for (long i = 0; i < pageNum; i++) {
		int mapNo = reqPage(head, pageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
		for (int j = 0; j < ph.curRecordNum; j++) {
			memcpy(&head->data_dict[count], head->buff.data[mapNo] + SIZE_PAGEHEAD + j * sizeof(Relation), sizeof(Relation));
			count++;
		}
		if (ph.nextPageNo < 0)
			break;
		else
			pageNo = ph.nextPageNo;
	}
	printf("Read the exist schema in current database successfully.\n");
	return count;
}