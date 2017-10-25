#include "storage.h"

#include <QFileDialog>
#include <QMessageBox> 
#include "MainWindow.h"

Storage::Storage()
{
}


Storage::~Storage()
{
} 

void Storage::createDbMetaHead(char *fileName){
	struct DbMetaHead sysHead;
	// 配置系统参数，并写入文件中

	// 存储文件中，数据区大小，默认为196MB
	sysHead.desc.fileSize = DATA_SPACE_SIZE;
	// 每页的大小，4KB
	sysHead.desc.pageSize = PAGE_SIZE;
	// 总页数
	sysHead.desc.pageSum = (DATA_SPACE_SIZE) / (PAGE_SIZE);  // 49 * 1024
	// 当前可用的页数，初始化时=pageSum
	sysHead.desc.pageFree = sysHead.desc.pageSum;

	// bitMap 空闲空间映射表
	sysHead.desc.bitMapAddStart = ADDR_BITMAP;    // bitMap在存储文件中的起始地址 =1024=1KB
	// bitMap的大小，每页用1bit，一共需要"页数"个bit，即"页数/8"个字节
	sysHead.desc.bitMapSize = sysHead.desc.pageSum / 8;	//byte

	sysHead.desc.dataAddStart = ADDR_DATA;     // 数据区起始位置，前面留给bitMap
	//sysHead.desc.nextfileID = 0;
	sysHead.desc.curFileNum = 0;
	//memset(sysHead.desc.fileInfo, -1, sizeof(struct FileInfo) * MAX_FILE_NUM);

	sysHead.desc.fileInfo[0].fileID = 0;
	sysHead.desc.fileInfo[0].fileName = 0;
	sysHead.desc.fileInfo[0].fileSegNum = SEG_NUM;
	sysHead.desc.fileInfo[0].fileState = 1;
	sysHead.desc.fileInfo[0].fileType = 0;
	memset(sysHead.desc.fileInfo[0].segTable, -1, sizeof(struct Segment) * SEG_NUM);

	FILE *fp = fopen(fileName, "wb");
	rewind(fp);
	fwrite(&(sysHead.desc), sizeof(struct DbInfo), 1, fp);// 把初始化的sysHead相关参数写到文件头部
	// 为空闲空间映射表分配空间，所有的初始化为-1，表示空闲
	sysHead.FreeSpace_bitMap = (unsigned long *)malloc(sysHead.desc.bitMapSize);
	memset(sysHead.FreeSpace_bitMap, -1, sysHead.desc.bitMapSize);

	// 再把bitMap写入到文件中
	rewind(fp);
	fseek(fp, sysHead.desc.bitMapAddStart, SEEK_SET);
	fwrite(sysHead.FreeSpace_bitMap, 1, sysHead.desc.bitMapSize, fp);

	fclose(fp);
}

void Storage::initDB(struct DbMetaHead *head, char *fileName, MainWindow* ui){
	FILE *fp; 
	fp = fopen(fileName, "rb"); 
	 
	if (fp == NULL){
		//printf("Database file is not exist, begin to create database file......\n\n");  
		QString info = QString::fromLocal8Bit("【") + "System Message Info" + QString::fromLocal8Bit("】");

		ui->sysBrowser->setText(info + "\n- - - - - - - - - - - -\n" + "Database file is not exist, begin to create database file.\n");
		 
		/*QString value = ui->sysBrowser->QTextBrowser::toPlainText();
		QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), value);*/
		  
		this->createDbMetaHead(fileName);
		fp = fopen(fileName, "rb");
	}
	//初始化head->desc
	rewind(fp);
	fread(&(head->desc), sizeof(struct DbInfo), 1, fp);
	//给bitMap分配空间，并初始化
	head->FreeSpace_bitMap = (unsigned long *)malloc(head->desc.bitMapSize);
	rewind(fp);
	fseek(fp, head->desc.bitMapAddStart, SEEK_SET); // 找到文件中写bitMap的位置，让fp指向这里
	fread(head->FreeSpace_bitMap, 1, head->desc.bitMapSize, fp);// 从文件中读取bitMap的内容
	fclose(fp);

	// 初始化缓冲区
	// 将每个缓冲区块都初始化（存储的文件的页号为-1，访问时间为0，标记为未修改过）
	for (int i = 0; i < BUFFER_Size; i++) {
		head->buff.map[i].pageNo = -1;
		head->buff.map[i].visitTime = 0;
		head->buff.map[i].isEdited = false;
		memset(head->buff.data[i], 0, PAGE_SIZE);
	}
	head->buff.curTimeStamp = 0;
	head->dataPath = fopen(fileName, "rb+"); 
}


void Storage::showDbInfo(struct DbMetaHead *head){
	printf("**************************************************\n\n");
	printf("fileSize:      %ld\n", (head->desc.fileSize));
	printf("pageSize:     %ld\n", (head->desc.pageSize));
	printf("pageSum:       %ld\n", (head->desc.pageSum));
	printf("pageFree:        %ld\n", (head->desc.pageFree));
	printf("bitMapAddStart:      %ld\n", (head->desc.bitMapAddStart));
	printf("bitMapSize:      %ld\n", (head->desc.bitMapSize));
	printf("dataAddStart:        %ld\n", (head->desc.dataAddStart));
	printf("curFileNum:      %d\n", (head->desc.curFileNum));
	printf("\n**************************************************\n");
}

void Storage::showSegTable(struct DbMetaHead *head, int fileID) {
	int fx = fileOpt.queryFileByIndex(head, fileID);
	long pageNum = head->desc.fileInfo[0].segTable[fx].segPageNum;
	long pageNo = head->desc.fileInfo[0].segTable[fx].segFirstPageNo;
	int type = head->desc.fileInfo[0].segTable[fx].segType;
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

//更新，把内存中的所有数据写入到磁盘中
int memToDisk(struct DbMetaHead *head){
	rewind(head->dataPath);
	fwrite(&(head->desc), sizeof(struct DbInfo), 1, head->dataPath);

	rewind(head->dataPath);
	fseek(head->dataPath, head->desc.bitMapAddStart, SEEK_SET);
	fwrite(head->FreeSpace_bitMap, sizeof(char), head->desc.bitMapSize, head->dataPath);

	for (int i = 0; i<BUFFER_Size; i++) {
		if (head->buff.map[i].isEdited == true) {
			rewind(head->dataPath);
			fseek(head->dataPath, head->desc.dataAddStart + PAGE_SIZE * head->buff.map[i].pageNo, SEEK_SET);
			fwrite(head->buff.data[i], sizeof(char), PAGE_SIZE, head->dataPath);
			head->buff.map[i].isEdited = false;
		}
	}
	return 0;
}