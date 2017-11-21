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
		printf("��ǰ���ݿ����Ѵ��ڹ���Ĺ�ϵ���޷������¹�ϵ��\n");
		return  -1;
	}

	return dictID;
}

int createTmpTable2(struct dbSysHead *head, Relation r1, Relation r2, int r1_pub_attr, int r2_pub_attr){
	int fid = createFile(head, TMP_TABLE, 1);
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0 && head->data_dict[i].attributeNum == 0){
			//�Ȱ�r1��ȫ���� head->data_dict[i]
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
		printf("��ǰ���ݿ����Ѵ��ڹ���Ĺ�ϵ���޷������¹�ϵ��\n");
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
		printf("��ǰ���ݿ����Ѵ��ڹ���Ĺ�ϵ���޷������¹�ϵ��\n");
		return  -1;
	}

	int rl_fid = rl.fileID;
	long pageNo = head->desc.fileDesc[rl_fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[rl_fid].filePageNum;
	    /**
	    *@Description:
	    *multimap����ӳ������
		*   multimap����ӳ������ : ���������ݽṹ���ú�������й���
		*   multimap������Ԫ�ض���pair : ��һԪ��Ϊ��ֵ(key), �����޸�; �ڶ�Ԫ��Ϊʵֵ(value), �ɱ��޸�
		*   multimap�����Լ��÷���map��ȫ��ͬ��Ψһ�Ĳ������:
	    *�����ظ���ֵ��Ԫ�ز�������(ʹ����RB - Tree��insert_equal����)
		*   ��� :
		*��ֵkey��Ԫ��value��ӳ�չ�ϵ�Ƕ�Զ�Ĺ�ϵ
		*      û�ж���[]��������
		*      ����key����������
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
			//��ʱ��ֻ������������Ϊint���͵�
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
			//��ʱֻ����Ҫ����hash������Ϊint���͵����
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


//����ʱ���в���һ����¼���������õĻ�������
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
		//��ҳͷ
		mapNo = reqPage(head, curPageNo);
		memcpy(&PHead, head->buff.data[mapNo], SIZE_PAGEHEAD);

		//��ҳ�Ŀ���ռ䲻����һ�����ݺ�һ��ƫ�����ˣ�����һҳ
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
			curOffset.logicID = -1;         //��ʱ���еļ�¼����Ҫ�����߼���
			curOffset.recordID = PHead.curRecordNum;
			curOffset.isDeleted = false;

			if (PHead.curRecordNum == 0) {   //��ҳ�������κμ�¼
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

			//����ҳͷ
			memcpy(head->buff.data[mapNo], &PHead, SIZE_PAGEHEAD);

			isFinished = true;
			break;
		}
	}
	//���ļ���û���ĸ�ҳ���㹻�Ŀռ����д�¸ü�¼����չ�ļ����ٷ���һ����ҳ
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

		//������ҳ��ҳͷ
		mapNo = reqPage(head, ph.pageNo);
		memcpy(head->buff.data[mapNo], &ph, SIZE_PAGEHEAD);

	}
	//����ƫ��������¼ͷ���ͼ�¼
	memcpy(head->buff.data[mapNo] + curOffsetPos, &curOffset, SIZE_OFFSET);
	memcpy(head->buff.data[mapNo] + curRecordPos, record, length);
	head->buff.map[mapNo].isEdited = true;
	head->data_dict[tmp_table_dictID].recordNum++;
	return mapNo;
}