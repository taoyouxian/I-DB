#include "dbHead.h"

int tableScanEqualSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* value){
	Relation rl = head->data_dict[dictID];
	int fid = rl.fileID;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[fid].filePageNum;

	int tmp_table_dictID = createTmpTable(head, rl);
	if (tmp_table_dictID < 0){
		printf("创建临时表失败！\n");
		return -1;
	}
	int attr_index = rl.getAttributeIndexByName(attribute_name);

	if (attr_index < 0){
		printf("关系表%s中没有名为%s的属性！\n", rl.relationName, attribute_name);
		return -1;
	}
	//按块遍历关系的每个元组
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
				//discard：保存到临时表中，临时表不存储到数据库中，而是单独的一个临时txt，当数据库关闭时删除该txt
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
		printf("创建临时表失败！\n");
		return -1;
	}
	int attr_index = rl.getAttributeIndexByName(attribute_name);
	if (attr_index < 0){
		printf("关系表%s中没有名为%s的属性！\n", rl.relationName, attribute_name);
		return -1;
	}
	//按块遍历关系的每个元组
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
			//判断进行范围选择的属性的类型
			if (rl.atb[attr_index].getType() == INT_TYPE){
				int min_ = atoi(min);
				int max_ = atoi(max);
				int val_ = atoi(val);
				if (val_ >= min_ && val_ <= max_)
					insertOneRecord(head, tmp_table_dictID, record);
			}
			//char类型
			else if (rl.atb[attr_index].getType() == CHAR_TYPE) {
				if (strcmp(min, val) <= 0 && strcmp(max, val) >= 0){
					insertOneRecord(head, tmp_table_dictID, record);
				}
			}
			//date类型，待实现
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