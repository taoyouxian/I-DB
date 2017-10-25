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
	// ����ϵͳ��������д���ļ���

	// �洢�ļ��У���������С��Ĭ��Ϊ196MB
	sysHead.desc.fileSize = DATA_SPACE_SIZE;
	// ÿҳ�Ĵ�С��4KB
	sysHead.desc.pageSize = PAGE_SIZE;
	// ��ҳ��
	sysHead.desc.pageSum = (DATA_SPACE_SIZE) / (PAGE_SIZE);  // 49 * 1024
	// ��ǰ���õ�ҳ������ʼ��ʱ=pageSum
	sysHead.desc.pageFree = sysHead.desc.pageSum;

	// bitMap ���пռ�ӳ���
	sysHead.desc.bitMapAddStart = ADDR_BITMAP;    // bitMap�ڴ洢�ļ��е���ʼ��ַ =1024=1KB
	// bitMap�Ĵ�С��ÿҳ��1bit��һ����Ҫ"ҳ��"��bit����"ҳ��/8"���ֽ�
	sysHead.desc.bitMapSize = sysHead.desc.pageSum / 8;	//byte

	sysHead.desc.dataAddStart = ADDR_DATA;     // ��������ʼλ�ã�ǰ������bitMap
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
	fwrite(&(sysHead.desc), sizeof(struct DbInfo), 1, fp);// �ѳ�ʼ����sysHead��ز���д���ļ�ͷ��
	// Ϊ���пռ�ӳ������ռ䣬���еĳ�ʼ��Ϊ-1����ʾ����
	sysHead.FreeSpace_bitMap = (unsigned long *)malloc(sysHead.desc.bitMapSize);
	memset(sysHead.FreeSpace_bitMap, -1, sysHead.desc.bitMapSize);

	// �ٰ�bitMapд�뵽�ļ���
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
		QString info = QString::fromLocal8Bit("��") + "System Message Info" + QString::fromLocal8Bit("��");

		ui->sysBrowser->setText(info + "\n- - - - - - - - - - - -\n" + "Database file is not exist, begin to create database file.\n");
		 
		/*QString value = ui->sysBrowser->QTextBrowser::toPlainText();
		QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), value);*/
		  
		this->createDbMetaHead(fileName);
		fp = fopen(fileName, "rb");
	}
	//��ʼ��head->desc
	rewind(fp);
	fread(&(head->desc), sizeof(struct DbInfo), 1, fp);
	//��bitMap����ռ䣬����ʼ��
	head->FreeSpace_bitMap = (unsigned long *)malloc(head->desc.bitMapSize);
	rewind(fp);
	fseek(fp, head->desc.bitMapAddStart, SEEK_SET); // �ҵ��ļ���дbitMap��λ�ã���fpָ������
	fread(head->FreeSpace_bitMap, 1, head->desc.bitMapSize, fp);// ���ļ��ж�ȡbitMap������
	fclose(fp);

	// ��ʼ��������
	// ��ÿ���������鶼��ʼ�����洢���ļ���ҳ��Ϊ-1������ʱ��Ϊ0�����Ϊδ�޸Ĺ���
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
	printf("�ļ�����:");
	if (type == NORMAL_FILE)
		printf("�û����ļ�\n");
	else
		printf("�����ļ�\n");
	printf("���ļ�һ��ռ�ã�%ldҳ\n", pageNum);
	printf("���ļ�����ʼҳҳ��Ϊ��%ld\n", pageNo);
	printf("**************************************************\n\n");
}

//���£����ڴ��е���������д�뵽������
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