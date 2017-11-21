#include "dbHead.h"

int projection(struct dbSysHead *head, int dictID, char* attribute_name){
	Relation rl = head->data_dict[dictID];
	int fid = rl.fileID;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[fid].filePageNum;

	//得到投影所指定的所有属性
	char *one_attribute_name = (char*)malloc(NAME_MAX_LENGTH);
	memset(one_attribute_name, 0, NAME_MAX_LENGTH);
	int k = 0, attrNum = 0;
	int *filedIndex = (int*)malloc(rl.attributeNum);
	memset(filedIndex, -1, rl.attributeNum);
	for (int i = 0; i <= strlen(attribute_name); i++) {
		if (attribute_name[i] == '|' || i == strlen(attribute_name)){
			filedIndex[attrNum++] = rl.getAttributeIndexByName(one_attribute_name);
			memset(one_attribute_name, 0, NAME_MAX_LENGTH);
			k = 0;
		}
		else
			one_attribute_name[k++] = attribute_name[i];
	}

	int tmp_table_dictID = createTmpTable(head, rl);
	if (tmp_table_dictID < 0){
		printf("创建临时表失败！\n");
		return -1;
	}
	for (int i = 0; i < pageNum; i++){
		int mapNo = reqPage(head, pageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
		//处理该页中的每一条记录
		for (int recordID = 0; recordID < ph.curRecordNum; recordID++) {

			char *record = (char*)malloc(rl.recordLength);
			getNextRecord(head, mapNo, recordID, record);

			char *result = (char*)malloc(rl.recordLength);
			memset(result, 0, rl.recordLength);

			char *one_filed = (char*)malloc(rl.recordLength);
			memset(one_filed, 0, rl.recordLength);

			for (int k = 0; k < attrNum; k++) {
				int p = getValueByAttrID(record, filedIndex[k], one_filed);
				if (p < 0){
					printf("bug in projection.cpp\n");
					exit(0);
				}
				strcat(result, one_filed);
				if (k < attrNum - 1)
					strcat(result, "|");
				else
					break;
			}
			insertOneRecord(head, tmp_table_dictID, result);
		}
		if (ph.nextPageNo <= 0)
			break;
		else
			pageNo = ph.nextPageNo;

	}
	return tmp_table_dictID;
}

int getValueByAttrID(char *str, int index, char *result){
	int length = strlen(str);
	/*result = (char*) malloc(length);
	memset(result, 0, length);*/
	int j = 0, k = 0;
	int start = 0;
	for (int i = 0; i <= length; i++) {
		if (str[i] == '|' || i == length){
			if (k == index){
				result[j] = '\0';
				return start;
			}
			else{
				start = i + 1;
				k++;
				memset(result, 0, length);
				j = 0;
			}
		}
		else
			result[j++] = str[i];
	}
	return -1;
}