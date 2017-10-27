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

void Storage::createDbMetaHead(char *fileName, MainWindow* ui){
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
		string s1;
		s1.append("Tiny DB> Message ");
		s1.append(to_string(ui->INFO_NUM));
		ui->INFO_NUM++;
		s1.append(": ");
		s1.append(GetDate());
		s1.append("\n\tDatabase file is not exist, creating.\n");
		QString info = QString::fromLocal8Bit("��") + "System Message Info" + QString::fromLocal8Bit("��") + "\n- - - - - - - - - - - -\n\n" + QString::fromStdString(s1);
		ui->sysBrowser->setText(info);
		this->createDbMetaHead(fileName, ui);
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
	for (int i = 0; i < BUFFER_SIZE; i++) {
		head->buff.map[i].pageNo = -1;
		head->buff.map[i].visitTime = 0;
		head->buff.map[i].isEdited = false;
		memset(head->buff.data[i], 0, PAGE_SIZE);
	}
	head->buff.curTimeStamp = 0;
	head->dataPath = fopen(fileName, "rb+");
}


void Storage::showDbInfo(struct DbMetaHead *head, MainWindow* ui){
	string s1; 
	s1.append("Tiny DB> Message ");
	s1.append(to_string(ui->INFO_NUM));
	s1.append(": ");
	s1.append(GetDate());
	s1.append("\n\tfileSize: ");
	s1.append(to_string(head->desc.fileSize));
	s1.append("\n\tpageSize: ");
	s1.append(to_string(head->desc.pageSize));
	s1.append("\n\tpageSum: " + to_string(head->desc.pageSum));
	s1.append("\n\tpageFree: " + to_string(head->desc.pageFree));
	s1.append("\n\tbitMapSize: " + to_string(head->desc.bitMapSize));
	s1.append("\n\tdataAddStart: " + to_string(head->desc.dataAddStart));
	s1.append("\n\tcurFileNum: " + to_string(head->desc.curFileNum));
	s1.append("\n");

	/*QString info = QString::fromLocal8Bit("��") + "System Message Info" + QString::fromLocal8Bit("��") + QString::fromStdString(s1);
	string s1;
	s1.insert(0, str);*/
	QString str;
	str = ui->sysBrowser->toPlainText() + QString::fromStdString(s1);
	ui->sysBrowser->setText(str);
	ui->INFO_NUM++;
}

void Storage::showSegTable(struct DbMetaHead *head, int fileID, MainWindow* ui) {
	int fx = fileOpt.queryFileByIndex(head, fileID, ui);
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

	for (int i = 0; i<BUFFER_SIZE; i++) {
		if (head->buff.map[i].isEdited == true) {
			rewind(head->dataPath);
			fseek(head->dataPath, head->desc.dataAddStart + PAGE_SIZE * head->buff.map[i].pageNo, SEEK_SET);
			fwrite(head->buff.data[i], sizeof(char), PAGE_SIZE, head->dataPath);
			head->buff.map[i].isEdited = false;
		}
	}
	return 0;
}