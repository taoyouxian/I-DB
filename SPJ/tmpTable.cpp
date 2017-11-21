#include "dbHead.h"

int createTmpTable(struct dbSysHead *head, Relation original_rl){
	int fid = createFile(head, TMP_TABLE, 1);
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			head->data_dict[i] = original_rl;
			head->data_dict[i].fileID = fid;
			strcat(head->data_dict[i].relationName, "_tmp");
			head->data_dict[i].recordNum = 0;
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("当前数据库中已存在过多的关系，无法创建新关系！\n");
		return  -1;
	}

	return dictID;
}

int createTmpTable2(struct dbSysHead *head, Relation r1, Relation r2, int r1_pub_attr, int r2_pub_attr){
	int fid = createFile(head, TMP_TABLE, 1);
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0 && head->data_dict[i].attributeNum == 0){
			//先把r1完全赋给 head->data_dict[i]
			head->data_dict[i] = r1;
			head->data_dict[i].fileID = fid;
			strcat(head->data_dict[i].relationName, "_");
			strcat(head->data_dict[i].relationName, r2.relationName);
			strcat(head->data_dict[i].relationName, "_tmp");
			head->data_dict[i].attributeNum = r1.attributeNum + r2.attributeNum - 1;
			int j = r1.attributeNum;
			for (int k = 0; k < r2.attributeNum; k++) {
				if (k != r2_pub_attr){
					head->data_dict[i].atb[j] = r2.atb[k];
					head->data_dict[i].isIndexed[j] = r2.isIndexed[k];
					head->data_dict[i].isOrdered[j] = r2.isOrdered[k];
					j++;
				}
			}
			head->data_dict[i].recordNum = 0;
			head->data_dict[i].recordLength = r1.recordLength + r2.recordLength - r1.atb[r1_pub_attr].getLength();

			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("当前数据库中已存在过多的关系，无法创建新关系！\n");
		return  -1;
	}
	return dictID;
}

bool isExisted(int *arr, int map, long length){
	for (int i = 0; i < length; i++) {
		if (arr[i] == map)
			return true;
	}
	return false;
}
int createTmpTableAfterSort(struct dbSysHead *head, Relation rl, int pub_attr){
	int fid = createFile(head, TMP_TABLE, 1);
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			head->data_dict[i] = rl;
			head->data_dict[i].fileID = fid;
			strcat(head->data_dict[i].relationName, "_tmp");
			head->data_dict[i].recordNum = 0;
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("当前数据库中已存在过多的关系，无法创建新关系！\n");
		return  -1;
	}

	int rl_fid = rl.fileID;
	long pageNo = head->desc.fileDesc[rl_fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[rl_fid].filePageNum;
	    /**
	    *@Description:
	    *multimap多重映照容器
		*   multimap多重映照容器 : 容器的数据结构采用红黑树进行管理
		*   multimap的所有元素都是pair : 第一元素为键值(key), 不能修改; 第二元素为实值(value), 可被修改
		*   multimap特性以及用法与map完全相同，唯一的差别在于:
	    *允许重复键值的元素插入容器(使用了RB - Tree的insert_equal函数)
		*   因此 :
		*键值key与元素value的映照关系是多对多的关系
		*      没有定义[]操作运算
		*      按照key进行了排序
		*/

	multimap<int, long> m;
	for (int i = 0; i < pageNum; i++) {
		int mapNo = reqPage(head, pageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);

		for (int j = 0; j < ph.curRecordNum; j++) {
			char *record = (char*)malloc(rl.recordLength);
			long logicID = getNextRecord(head, mapNo, j, record);
			if (logicID < 0){
				cout << logicID << endl;
			}
			char *val = (char*)malloc(rl.recordLength);
			getValueByAttrID(record, pub_attr, val);
			//暂时先只考虑排序属性为int类型的
			int tmp = atoi(val);
			m.insert(pair<int, long>(tmp, logicID));
		}

		if (ph.nextPageNo < 0)
			break;
		else
			pageNo = ph.nextPageNo;
	}
	//    int count = 0;
	while (!m.empty()) {
		multimap<int, long>::iterator it = m.begin();
		char* des = (char*)malloc(rl.recordLength);

		queryRecordByLogicID(head, it->second, des);
		insertOneRecord(head, dictID, des);
		//        if(count == -1 || isExisted(buffNo, mapNo, head->desc.fileDesc[queryFileIndex(head, rl.fileID)].filePageNum) == false){
		//            buffNo[count] =mapNo;
		//            count++;
		//            head->buff.map[mapNo].isPin = true;
		//        }        
		m.erase(it);
	}
	return dictID;
}

int hashToBucket(int val){
	return val % BUCKET_NUM;
}

void HashRelation(struct dbSysHead *head, Relation rl, int pub_attr, multimap<int, long> *m) {
	int rl_fid = rl.fileID;
	long pageNo = head->desc.fileDesc[rl_fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[rl_fid].filePageNum;

	for (int i = 0; i < pageNum; i++) {
		int mapNo = reqPage(head, pageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
		for (int j = 0; j < ph.curRecordNum; j++) {
			char *record = (char*)malloc(rl.recordLength);
			long logicID = getNextRecord(head, mapNo, j, record);
			char *val = (char*)malloc(rl.recordLength);
			getValueByAttrID(record, pub_attr, val);
			//暂时只考虑要进行hash的属性为int类型的情况
			int int_val = atoi(val);
			int bid = hashToBucket(int_val);
			m[bid].insert(pair<int, long>(int_val, logicID));
		}
		if (ph.nextPageNo < 0)
			break;
		else
			pageNo = ph.nextPageNo;
	}
}


//向临时表中插入一条记录，返回所用的缓冲区号
int insertOneRecord(struct dbSysHead *head, int tmp_table_dictID, char* record){
	int length = strlen(record);
	int fid = head->data_dict[tmp_table_dictID].fileID;
	long curPageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[fid].filePageNum;

	struct pageHead PHead;
	int mapNo = 0;
	long curRecordPos = 0, curOffsetPos = 0;
	struct offsetInPage curOffset, preOffset;

	bool isFinished = false;
	for (int i = 0; i < pageNum; i++) {
		//读页头
		mapNo = reqPage(head, curPageNo);
		memcpy(&PHead, head->buff.data[mapNo], SIZE_PAGEHEAD);

		//本页的空余空间不够加一条数据和一个偏移量了，找下一页
		if (PHead.freeSpace <= SIZE_OFFSET + length){
			long nextpid = PHead.nextPageNo;
			if (nextpid == -1)
				break;
			else {
				curPageNo = nextpid;
				continue;
			}
		}
		else {
			curOffset.logicID = -1;         //临时表中的记录不需要分配逻辑号
			curOffset.recordID = PHead.curRecordNum;
			curOffset.isDeleted = false;

			if (PHead.curRecordNum == 0) {   //该页中暂无任何记录
				curOffset.offset = length;
				curRecordPos = SIZE_PER_PAGE - length;
				curOffsetPos = SIZE_PAGEHEAD;
			}
			else {
				memcpy(&preOffset, head->buff.data[mapNo] + SIZE_PAGEHEAD + SIZE_OFFSET * (PHead.curRecordNum - 1), SIZE_OFFSET);
				curOffset.offset = preOffset.offset + length;
				curRecordPos = SIZE_PER_PAGE - preOffset.offset - length;
				curOffsetPos = SIZE_PAGEHEAD + SIZE_OFFSET * curOffset.recordID;
			}
			PHead.curRecordNum++;
			PHead.freeSpace = PHead.freeSpace - length - SIZE_OFFSET;

			//更新页头
			memcpy(head->buff.data[mapNo], &PHead, SIZE_PAGEHEAD);

			isFinished = true;
			break;
		}
	}
	//该文件中没有哪个页有足够的空间可以写下该记录，扩展文件，再分配一个新页
	if (isFinished == false) {
		struct pageHead ph = extendFile(head, fid, &PHead);

		curOffset.logicID = -1;
		curOffset.recordID = 0;
		curOffset.offset = length;
		curOffset.isDeleted = false;

		curRecordPos = SIZE_PER_PAGE - length;
		curOffsetPos = SIZE_PAGEHEAD;

		ph.curRecordNum = 1;
		ph.freeSpace = ph.freeSpace - SIZE_OFFSET - length;

		//更新新页的页头
		mapNo = reqPage(head, ph.pageNo);
		memcpy(head->buff.data[mapNo], &ph, SIZE_PAGEHEAD);

	}
	//插入偏移量、记录头部和记录
	memcpy(head->buff.data[mapNo] + curOffsetPos, &curOffset, SIZE_OFFSET);
	memcpy(head->buff.data[mapNo] + curRecordPos, record, length);
	head->buff.map[mapNo].isEdited = true;
	head->data_dict[tmp_table_dictID].recordNum++;
	return mapNo;
}