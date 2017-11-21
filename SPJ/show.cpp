#include "dbHead.h"

void show_SysDesc(struct dbSysHead *head){
	printf("\n******************* ���ݿ����ز��� ****************\n");
	printf("���ݿ��ܴ�С��%ld\n", head->desc.sizeOfFile);
	printf("ÿһҳ�Ĵ�С��%ld\n", head->desc.sizePerPage);
	printf("���ݿ���ҳ����%ld\n", head->desc.TotalPage);
	printf("��ǰ����ҳ����%ld\n", head->desc.AvaiPage);
	printf("���пռ�λʾͼ����ʼ��ַ��%ld\n", head->desc.addr_FreeSpace_BitMap);
	printf("���пռ�λʾͼ�Ĵ�С��%ld\n", head->desc.size_FreeSpace_BitMap);
	printf("�����߼���λʾͼ����ʼ��ַ��%ld\n", head->desc.addr_FreeLogicID_BitMap);
	printf("�����߼���λʾͼ�Ĵ�С��%ld\n", head->desc.size_FreeLogicID_BitMap);
	printf("����������ʼ��ַ��%ld\n", head->desc.addr_data);
	printf("ӳ����ļ����ļ��ţ�%d\n", head->desc.fid_MapTable);
	printf("�����ֵ��ļ����ļ��ţ�%d\n", head->desc.fid_DataDictionary);
	printf("��ǰ���ݿ���ļ�������%d\n", head->desc.curFileNum);
	printf("��ǰ���ݿ��еļ�¼������%ld\n", head->desc.curRecordNum);
	printf("\n**************************************************\n");
}

void show_Relation(struct dbSysHead *head, int dictID) {
	Relation rl = head->data_dict[dictID];
	printf("������%s\n", rl.relationName);
	printf("���Ը�����%d\n", rl.attributeNum);
	printf("��¼���ȣ�%d\n", rl.recordLength);
	printf("��¼������%d\n", rl.recordNum);
}

void show_FileDesc(struct dbSysHead *head, int dictID) {
	int fid = head->data_dict[dictID].fileID;
	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
		return;
	}
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	int type = head->desc.fileDesc[fid].fileType;
	printf("\n**************************************************\n");
	printf("�ļ�����:");
	if (type == MAP_FILE)
		printf("ӳ����ļ�\n");
	else if (type == USER_FILE){
		printf("�û����ļ�\n");
		show_Relation(head, dictID);
	}
	else
		printf("�����ļ�\n");
	printf("���ļ�һ��ռ�ã�%ldҳ\n", pageNum);
	printf("���ļ�����ʼҳҳ��Ϊ��%ld\n", pageNo);
	printf("\n**************************************************\n");

}

void show_MapTableFile(struct dbSysHead *head, int fid) {
	show_FileDesc(head, fid);
	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
		return;
	}
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	long curPageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long size_ph = sizeof(struct pageHead);
	long size_mt = sizeof(struct dbMapTable);
	printf("\n**************************************************\n");
	printf("�߼���ַ    �����ַ\n");
	printf("�߼���    ҳ��|ҳ�ڼ�¼��\n");
	for (int i = 0; i < pageNum; i++) {
		int mapNo = reqPage(head, curPageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], size_ph);
		for (int j = 0; j < ph.curRecordNum; j++) {
			struct dbMapTable mt;
			memcpy(&mt, head->buff.data[mapNo] + size_ph + j * size_mt, size_mt);
			printf("%ld\t%ld\t%ld\n", mt.logicID, mt.pageNo, mt.recordID);
		}
		long nextpid = ph.nextPageNo;
		if (nextpid < 0)
			break;
		else
			curPageNo = nextpid;
	}
}

void welcome(){
	printf("**************************************************\n");
	printf("**************************************************\n");
	printf("**********                               *********\n");
	printf("**********   ��ӭʹ�����ǵ����ݿ����ϵͳ��    *********\n");
	printf("**********                               *********\n");
	printf("**************************************************\n");
	printf("**************************************************\n");
}

void exitdb(){
	printf("**************************************************\n");
	printf("**************************************************\n");
	printf("**********                               *********\n");
	printf("**********   ллʹ�����ǵ����ݿ����ϵͳ��    *********\n");
	printf("**********                               *********\n");
	printf("**************************************************\n");
	printf("**************************************************\n");
}