#include "dbHead.h"

int tableScanEqualSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* value){
	Relation rl = head->data_dict[dictID];
	int fid = rl.fileID;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[fid].filePageNum;

	int tmp_table_dictID = createTmpTable(head, rl);
	if (tmp_table_dictID < 0){
		printf("������ʱ��ʧ�ܣ�\n");
		return -1;
	}
	int attr_index = rl.getAttributeIndexByName(attribute_name);

	if (attr_index < 0){
		printf("��ϵ��%s��û����Ϊ%s�����ԣ�\n", rl.relationName, attribute_name);
		return -1;
	}
	//���������ϵ��ÿ��Ԫ��
	for (int i = 0; i < pageNum; i++){
		int mapNo = reqPage(head, pageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
		for (int j = 0; j < ph.curRecordNum; j++) {

			char *record = (char*)malloc(rl.recordLength);
			getNextRecord(head, mapNo, j, record);
			char *val = (char*)malloc(strlen(record));
			memset(val, 0, strlen(record));
			int s = getValueByAttrID(record, attr_index, val);
			if (s < 0){
				printf("bug in join.cpp\n");
				exit(0);
			}
			if (strcmp(val, value) == 0){
				//discard�����浽��ʱ���У���ʱ���洢�����ݿ��У����ǵ�����һ����ʱtxt�������ݿ�ر�ʱɾ����txt
				insertOneRecord(head, tmp_table_dictID, record);
			}
		}
		if (ph.nextPageNo <= 0)
			break;
		else
			pageNo = ph.nextPageNo;
	}

	return tmp_table_dictID;
}

int tableScanRangeSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* min, char* max){
	Relation rl = head->data_dict[dictID];
	int fid = rl.fileID;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[fid].filePageNum;

	int tmp_table_dictID = createTmpTable(head, rl);
	if (tmp_table_dictID < 0){
		printf("������ʱ��ʧ�ܣ�\n");
		return -1;
	}
	int attr_index = rl.getAttributeIndexByName(attribute_name);
	if (attr_index < 0){
		printf("��ϵ��%s��û����Ϊ%s�����ԣ�\n", rl.relationName, attribute_name);
		return -1;
	}
	//���������ϵ��ÿ��Ԫ��
	for (int i = 0; i < pageNum; i++){
		int mapNo = reqPage(head, pageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
		for (int j = 0; j < ph.curRecordNum; j++) {
			char *record = (char*)malloc(rl.recordLength);
			getNextRecord(head, mapNo, j, record);
			char *val = (char*)malloc(rl.recordLength);
			memset(val, 0, rl.recordLength);
			int s = getValueByAttrID(record, attr_index, val);
			if (s < 0){
				printf("bug in join.cpp\n");
				exit(0);
			}
			//�жϽ��з�Χѡ������Ե�����
			if (rl.atb[attr_index].getType() == INT_TYPE){
				int min_ = atoi(min);
				int max_ = atoi(max);
				int val_ = atoi(val);
				if (val_ >= min_ && val_ <= max_)
					insertOneRecord(head, tmp_table_dictID, record);
			}
			//char����
			else if (rl.atb[attr_index].getType() == CHAR_TYPE) {
				if (strcmp(min, val) <= 0 && strcmp(max, val) >= 0){
					insertOneRecord(head, tmp_table_dictID, record);
				}
			}
			//date���ͣ���ʵ��
			else{

			}
		}
		if (ph.nextPageNo <= 0)
			break;
		else
			pageNo = ph.nextPageNo;
	}

	return tmp_table_dictID;
}