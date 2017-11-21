#include "dbHead.h"

void creatSysHead(char *filename){
	struct dbSysHead sysHead; // ����ϵͳ��������д���ļ���

	// �洢�ļ��У���������С��Ĭ��Ϊ196MB
	sysHead.desc.sizeOfFile = SIZE_DATA_SPACE;
	// ÿҳ�Ĵ�С��4KB
	sysHead.desc.sizePerPage = SIZE_PER_PAGE;
	// ��ҳ��
	sysHead.desc.TotalPage = (SIZE_DATA_SPACE) / (SIZE_PER_PAGE);
	// ��ǰ���õ�ҳ������ʼ��ʱ=totalPage
	sysHead.desc.AvaiPage = sysHead.desc.TotalPage;

	// ���пռ�λʾͼ����ʼ��ַ
	sysHead.desc.addr_FreeSpace_BitMap = ADDR_FREE_SPACE_BIT_MAP;
	// ��С��ÿҳ��1bit��һ����Ҫ"ҳ��"��bit����"ҳ��/8"���ֽ�
	sysHead.desc.size_FreeSpace_BitMap = sysHead.desc.TotalPage / 8;	//byte

	// �����߼���λʾͼ����ʼ��ַ
	sysHead.desc.addr_FreeLogicID_BitMap = ADDR_FREE_LOGICID_BIT_MAP;
	// ���С����ʱ��̬����1024000�������߼���
	sysHead.desc.size_FreeLogicID_BitMap = SIZE_FREE_LOGICID_BIT_MAP;

	sysHead.desc.addr_data = ADDR_DATA;     //��������ʼλ�ã�ǰ����������bitMap
	sysHead.desc.fid_MapTable = -1;
	sysHead.desc.fid_DataDictionary = -1;

	sysHead.desc.curRecordNum = 0;                      //��ǰ���еļ�¼����
	sysHead.desc.curFileNum = 0;
	memset(sysHead.desc.fileDesc, -1, sizeof(struct FileDesc) * MAX_FILE_NUM);


	FILE *fp = fopen(filename, "wb");
	rewind(fp);
	fwrite(&(sysHead.desc), sizeof(struct SysDesc), 1, fp);// �ѳ�ʼ����sysHead��ز���д���ļ�ͷ��
	// Ϊ���пռ�ӳ������ռ䣬���еĳ�ʼ��Ϊ-1����ʾ����
	sysHead.FreeSpace_bitMap = (unsigned long *)malloc(sysHead.desc.size_FreeSpace_BitMap);
	memset(sysHead.FreeSpace_bitMap, -1, sysHead.desc.size_FreeSpace_BitMap);

	sysHead.FreeLogicID_bitMap = (unsigned long *)malloc(sysHead.desc.size_FreeLogicID_BitMap);
	memset(sysHead.FreeLogicID_bitMap, -1, sysHead.desc.size_FreeLogicID_BitMap);

	// ��д�뵽�ļ���
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
	//��ʼ��head->desc
	rewind(fp);
	fread(&(head->desc), sizeof(struct SysDesc), 1, fp);
	//����ռ䣬����ʼ��
	head->FreeSpace_bitMap = (unsigned long *)malloc(head->desc.size_FreeSpace_BitMap);
	rewind(fp);
	fseek(fp, head->desc.addr_FreeSpace_BitMap, SEEK_SET); // �ҵ��ļ���дbitMap��λ�ã���fpָ������
	fread(head->FreeSpace_bitMap, 1, head->desc.size_FreeSpace_BitMap, fp);// ���ļ��ж�ȡbitMap������

	head->FreeLogicID_bitMap = (unsigned long*)malloc(head->desc.size_FreeLogicID_BitMap);
	fseek(fp, head->desc.addr_FreeLogicID_BitMap, SEEK_SET);
	fread(head->FreeLogicID_bitMap, 1, head->desc.size_FreeLogicID_BitMap, fp);

	fclose(fp);
	head->fpdesc = fopen(filename, "rb+");

	// ��ʼ��������
	// ��ÿ���������鶼��ʼ�����洢���ļ���ҳ��Ϊ-1������ʱ��Ϊ0�����Ϊδ�޸Ĺ���
	for (int i = 0; i < SIZE_BUFF; i++) {
		head->buff.map[i].pageNo = -1;
		head->buff.map[i].visitTime = 0;
		head->buff.map[i].isEdited = false;
		head->buff.map[i].isPin = false;
		memset(head->buff.data[i], 0, SIZE_PER_PAGE);
	}
	head->buff.curTimeStamp = 0;

	int count = 0;
	//�������ֵ��ļ��ж�ȡ��ϵģʽ���ٳ�ʼ��data_dict
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