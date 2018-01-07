#include "SPJ.h"
#include "storage.h"  

SPJ::SPJ()
{
}


SPJ::~SPJ()
{
} 

FileOpt fileOpt;
PageOpt pageOpt;
BufOpt bufOpt;
 
/*
recordOption.cpp
 */
//�����߼��ţ��ҵ�����ӳ����е�����ҳ�����ظ�ҳҳͷ
long SPJ::queryLogicID(struct dbSysHead *head, long logicID, struct pageHead *PHead){ 
	long size_maptable = sizeof(struct dbMapTable);
	long num = (SIZE_PER_PAGE - SIZE_PAGEHEAD) / size_maptable;
	long page = logicID / num;
	long pos = logicID - num * page;

	int fid = head->desc.fid_MapTable;
	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
		exit(0);
	}
	long curPageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long filepagenum = head->desc.fileDesc[fid].filePageNum;

	//�ҵ���pageҳ��ҳͷPHead
	int i = 0;
	int mapNo;
	do{
		//��ҳͷ
		//todo��Ӧ�ð�ӳ��������ҳ��pin�ڻ������У�ʹ�䲻�ᱻ�滻
		mapNo = bufOpt.reqPage(head, curPageNo);
		memcpy(PHead, head->buff.data[mapNo], SIZE_PAGEHEAD);
		long nextpid = PHead->nextPageNo;
		if (nextpid == -1)   //�����һҳ��
			break;
		else
			curPageNo = nextpid;

		i++;
	} while (i <= page && i < filepagenum);

	return pos;
}
int SPJ::queryRecordByLogicID(struct dbSysHead *head, long logicID, char *des) {
	long size_maptable = sizeof(struct dbMapTable);
	long page = logicID / (8 * sizeof(long));
	long pos = logicID - 8 * sizeof(long)*page + 1;
	if (pageOpt.getBit(*(head->FreeLogicID_bitMap + page), pos) == LOGICID_AVAI){
		printf("�߼���%ld��δ���䣡\n", logicID);
		return -1;
	}
	//�õ��ü�¼��Ӧ��ӳ���������ڵ�ҳͷ���Լ��ñ����ڸ�ҳ�е�λ��
	struct pageHead PHead;
	pos = queryLogicID(head, logicID, &PHead);
	int mapNo = bufOpt.reqPage(head, PHead.pageNo);

	struct dbMapTable mt;
	memcpy(&mt, head->buff.data[mapNo] + SIZE_PAGEHEAD + size_maptable * pos, size_maptable);
	if (mt.isdeleted){
		printf("�߼���Ϊ%ld�ļ�¼�ѱ�ɾ�����޷���ȡ��\n", logicID);
		return -1;
	}
	long pageNo = mt.pageNo;
	long recordID = mt.recordID;
	mapNo = bufOpt.reqPage(head, pageNo);

	struct offsetInPage cur, pre;
	memcpy(&cur, head->buff.data[mapNo] + SIZE_PAGEHEAD + recordID * SIZE_OFFSET, SIZE_OFFSET);

	if (recordID > 0){
		memcpy(&pre, head->buff.data[mapNo] + SIZE_PAGEHEAD + (recordID - 1) * SIZE_OFFSET, SIZE_OFFSET);
		memcpy(des, head->buff.data[mapNo] + SIZE_PER_PAGE - cur.offset, cur.offset - pre.offset);
	}
	else
		memcpy(des, head->buff.data[mapNo] + SIZE_PER_PAGE - cur.offset, cur.offset);

	des[recordID > 0 ? cur.offset - pre.offset : cur.offset] = '\0';
	return 0;
}
//�ƶ���¼�����Ӧ�������ַ���б仯��ҳ��+��¼�ţ�
//�����߼��ţ���������ӳ����еı���
//void updateMapTableByLogicID(struct dbSysHead *head, long logicID, long pageNo, long recordID) {
//    long size_maptable = sizeof(struct dbMapTable);
//    long page = logicID / (8*sizeof(long));
//    long pos = logicID - 8*sizeof(long)*page + 1;
//    
//    struct pageHead PHead = queryLogicID(head, logicID);
//    int mapNo =bufOpt.reqPage(head, PHead.pageNo);
//    struct dbMapTable mt;
//    memcpy(&mt, head->buff.data[mapNo] + SIZE_PAGEHEAD + size_maptable * pos, size_maptable);
//    if(mt.isdeleted){
//        printf("�߼���Ϊ%ld�ļ�¼�ѱ�ɾ�����޷���ȡ��\n",logicID);
//        return;
//    }
//    mt.pageNo = pageNo;
//    mt.recordID = recordID;
//    memcpy(head->buff.data[mapNo] + SIZE_PAGEHEAD + size_maptable * pos, &mt, size_maptable);
//}
// �����߼��ţ����¸ü�¼������
void SPJ::updateRecordByLogicID(struct dbSysHead *head, long logicID, char* str) {

}
//��������
void SPJ::queryRecordByKey(struct dbSysHead *head, int fid, int key, char *des){

}
void SPJ::insertRecordByKey(struct dbSysHead *head, int fid, char *str) {

}
void SPJ::deleteRecordByKey(struct dbSysHead *head, int fid, int key) {

}
//����ҳ�ڼ�¼�ţ���ü�¼���ݣ������߼���
long SPJ::getNextRecord(struct dbSysHead *head, int mapNo, int recordNo, char *des){
	struct offsetInPage curoffset;
	int length = 0;
	memcpy(&curoffset, head->buff.data[mapNo] + SIZE_PAGEHEAD + recordNo * SIZE_OFFSET, SIZE_OFFSET);
	long logicID = curoffset.logicID;
	if (recordNo == 0)
		length = curoffset.offset;
	else {
		struct offsetInPage preoffset;
		memcpy(&preoffset, head->buff.data[mapNo] + SIZE_PAGEHEAD + (recordNo - 1) * SIZE_OFFSET, SIZE_OFFSET);
		length = curoffset.offset - preoffset.offset;
	}
	try{
		memcpy(des, head->buff.data[mapNo] + SIZE_PER_PAGE - curoffset.offset, length);
	}
	catch (exception e){
		cout << e.what() << endl;
	}
	des[length] = '\0';
	return logicID;
}
long SPJ::allocateLogicID(struct dbSysHead *head){
	long logicID;
	if (head->desc.curRecordNum >= MAX_LOGICID_NUM) {
		printf("�����ݿ��пɲ���ļ�¼���Ѵﵽ����%d���޷������¼�¼��\n", MAX_LOGICID_NUM);
		exit(0);
	}

	long page, pos;
	int count = 0;
	long i;
	long total = MAX_LOGICID_NUM;

	for (i = 0; i < total; i++) {
		page = i / (8 * sizeof(long));
		pos = i - 8 * sizeof(long) * page + 1;
		count = 0;
		if (pageOpt.getBit(*(head->FreeLogicID_bitMap + page), pos) == LOGICID_AVAI) {
			count = 1;
			break;
		}
	}
	if (count != 1) {
		return ALLO_FAIL;
	}
	else {
		logicID = i;
		page = i / (8 * sizeof(long));
		pos = i - 8 * sizeof(long) * page + 1;
		pageOpt.setBit(head->FreeLogicID_bitMap + page, pos, LOGICID_UNAVAI);
		head->desc.curRecordNum++;
	}
	return logicID;
}
void SPJ::recyOneLogicID(struct dbSysHead *head, long logicID){
	long page = logicID / (8 * sizeof(long));
	long pos = logicID - 8 * sizeof(long)*page + 1;
	if (pageOpt.getBit(*(head->FreeLogicID_bitMap + page), pos) == LOGICID_UNAVAI){
		pageOpt.setBit(head->FreeLogicID_bitMap + page, pos, LOGICID_AVAI);
	}
}
// ����һ��ӳ����ļ�������ҳ������ÿ��isDeleted=true���߼���
void SPJ::recyAllFreeLogicID(struct dbSysHead *head){
	int fid = head->desc.fid_MapTable;
	if (fid < 0) {
		printf("���ݿ��в�����ӳ����ļ���\n");
		return;
	}
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	long curPageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long size_mt = sizeof(struct dbMapTable);
	for (int i = 0; i < pageNum; i++) {
		int mapNo = bufOpt.reqPage(head, curPageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
		for (int j = 0; j < ph.curRecordNum; j++) {
			struct dbMapTable mt;
			memcpy(&mt, head->buff.data[mapNo] + SIZE_PAGEHEAD + j * size_mt, size_mt);
			if (mt.isdeleted == true){
				recyOneLogicID(head, mt.logicID);
			}
		}
		if (ph.nextPageNo < 0)
			break;
		else
			curPageNo = ph.nextPageNo;
	}

}
/*
recordOption.cpp
*/
/*
tableOption.cpp
*/

//�������������������ֵ��е��±�
int SPJ::createTable_employee(struct dbSysHead *head) {
	int fid = fileOpt.createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("Failed to create user file.");
		return  -1;
	}
	else
		printf("Create user file %d successfully.\n\n", fid);

	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
		return -1;
	}
	//�������ֵ�����һ����λ
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("��ǰ���ݿ����Ѵ��ڹ���Ĺ�ϵ���޷������¹�ϵ��\n");
		return  -1;
	}
	/* todo:

	char* table_pattern;
	printf("��ϵͳ�Ļ����﷨�ɲο�SQL��\n");
	printf("�Դ���������Ϊ����\n");
	printf("create table employee(rid INT, did INT, rname VARCHAR(20), age INT, PRIMARY KEY(rid));\n");
	printf("������ʾ������һ����Ϊemployee�ı�\n");
	printf("�����������ֶΣ��ֱ���int���͵�rid��age���Լ�varchar���͵�name\n");
	printf("����ridΪ�ñ������\n");
	printf("���棬�봴�����ı�");
	scanf("%s", table_pattern);

	* дһ��parser����table_pattern��create��䣩
	* �õ���relationName,attributeName��attributeType
	* ����attributeName��attributeType�������Attribute�����������������Ա�
	* �ٹ���Relation����
	* Ŀǰ���ֶ�����relation
	*/
	head->data_dict[dictID].initRelation(head, fid, "employee");
	head->data_dict[dictID].insertAttribute("rid", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("did", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("age", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("rname", CHAR_TYPE, 20);

	return dictID;
}

//�������������������ֵ��е��±�
int SPJ::createTable_employee(struct dbSysHead *head, string sql) {
	int fid = fileOpt.createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("Failed to create user file.");
		return  -1;
	}
	else
		printf("Create user file %d successfully.\n\n", fid);

	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
		return -1;
	}
	//�������ֵ�����һ����λ
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("��ǰ���ݿ����Ѵ��ڹ���Ĺ�ϵ���޷������¹�ϵ��\n");
		return  -1;
	}
	/* todo:

	char* table_pattern;
	printf("��ϵͳ�Ļ����﷨�ɲο�SQL��\n");
	printf("�Դ���������Ϊ����\n");
	printf("create table employee(rid INT, did INT, rname VARCHAR(20), age INT, PRIMARY KEY(rid));\n");
	printf("������ʾ������һ����Ϊemployee�ı�\n");
	printf("�����������ֶΣ��ֱ���int���͵�rid��age���Լ�varchar���͵�name\n");
	printf("����ridΪ�ñ������\n");
	printf("���棬�봴�����ı�");
	scanf("%s", table_pattern);

	* дһ��parser����table_pattern��create��䣩
	* �õ���relationName,attributeName��attributeType
	* ����attributeName��attributeType�������Attribute�����������������Ա�
	* �ٹ���Relation����
	* Ŀǰ���ֶ�����relation
	*/
	string table_name;
	map<string, pair<int, int> > paras_map;
	createSql(sql, table_name, paras_map);
	map<string, pair<int, int> >::iterator it;
	it = paras_map.begin();
	//cout << table_name << endl;
	head->data_dict[dictID].initRelation(head, fid, table_name.c_str());

	while (it != paras_map.end())
	{
		/*cout << it->first << endl;
		pair<int, int> p1 = it->second;
		cout << p1.first << ' ' << p1.second << endl;*/
		const char *expr = it->first.c_str();
		char *buf = new char[strlen(expr) + 1];
		strcpy(buf, expr);

		pair<int, int> p1 = it->second;
		if (p1.second == -1){
			head->data_dict[dictID].insertAttribute(buf, p1.first, 4);
		}
		else{
			head->data_dict[dictID].insertAttribute(buf, p1.first, p1.second);
		}
		it++;
	}
	/*head->data_dict[dictID].insertAttribute("rid", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("did", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("age", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("rname", CHAR_TYPE, 20);*/

	return dictID;
}

//�������������������ֵ��е��±�
int SPJ::createTable_department(struct dbSysHead *head) {
	int fid = fileOpt.createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("�����û��ļ�ʧ�ܣ�");
		return  -1;
	}
	else
		printf("Create user file %d successfully.\n\n", fid);

	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
		return -1;
	}
	//�������ֵ�����һ����λ
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("��ǰ���ݿ����Ѵ��ڹ���Ĺ�ϵ���޷������¹�ϵ��\n");
		return  -1;
	}

	head->data_dict[dictID].initRelation(head, fid, "department");
	head->data_dict[dictID].insertAttribute("did", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("manager_id", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("num", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("dname", CHAR_TYPE, 20);
	head->data_dict[dictID].insertAttribute("idcard", CHAR_TYPE, 18);

	return dictID;
}

//�������������������ֵ��е��±�
int SPJ::createTable_birthday(struct dbSysHead *head) {
	int fid = fileOpt.createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("�����û��ļ�ʧ�ܣ�");
		return  -1;
	}
	else
		printf("Create user file %d successfully.\n\n", fid);

	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
		return -1;
	}
	//�������ֵ�����һ����λ
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("��ǰ���ݿ����Ѵ��ڹ���Ĺ�ϵ���޷������¹�ϵ��\n");
		return  -1;
	}

	head->data_dict[dictID].initRelation(head, fid, "birthday");
	head->data_dict[dictID].insertAttribute("idcard", CHAR_TYPE, 18);
	head->data_dict[dictID].insertAttribute("birthtime", CHAR_TYPE, 15);
	head->data_dict[dictID].insertAttribute("addressid", INT_TYPE, 2);

	return dictID;
}

//�������������������ֵ��е��±�
int SPJ::createTable_supplier(struct dbSysHead *head) {
	int fid = fileOpt.createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("�����û��ļ�ʧ�ܣ�");
		return  -1;
	}
	else
		printf("Create user file %d successfully.\n\n", fid);

	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
		return -1;
	}
	//�������ֵ�����һ����λ
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("��ǰ���ݿ����Ѵ��ڹ���Ĺ�ϵ���޷������¹�ϵ��\n");
		return  -1;
	}

	head->data_dict[dictID].initRelation(head, fid, "supplier");
	head->data_dict[dictID].insertAttribute("S_superkey", INT_TYPE, 10);
	head->data_dict[dictID].insertAttribute("S_name", CHAR_TYPE, 30);
	head->data_dict[dictID].insertAttribute("S_address", CHAR_TYPE, 40);
	head->data_dict[dictID].insertAttribute("S_nationkey", INT_TYPE, 10);
	head->data_dict[dictID].insertAttribute("S_phone", CHAR_TYPE, 20);
	head->data_dict[dictID].insertAttribute("S_acctbal", FLOAT_TYPE, 10);
	head->data_dict[dictID].insertAttribute("S_comment", CHAR_TYPE, 110);

	return dictID;
}

//�������������������ֵ��е��±�
int SPJ::createTable_partsupp(struct dbSysHead *head) {
	int fid = fileOpt.createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("�����û��ļ�ʧ�ܣ�");
		return  -1;
	}
	else
		printf("Create user file %d successfully.\n\n", fid);

	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ���\n", fid);
		return -1;
	}
	//�������ֵ�����һ����λ
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("��ǰ���ݿ����Ѵ��ڹ���Ĺ�ϵ���޷������¹�ϵ��\n");
		return  -1;
	}

	head->data_dict[dictID].initRelation(head, fid, "partsupp");
	head->data_dict[dictID].insertAttribute("PS_PARTKEY", INT_TYPE, 10);
	head->data_dict[dictID].insertAttribute("S_superkey", INT_TYPE, 10);
	head->data_dict[dictID].insertAttribute("PS_AVAILQTY", INT_TYPE, 10);
	head->data_dict[dictID].insertAttribute("PS_SUPPLYCOST", FLOAT_TYPE, 10);
	head->data_dict[dictID].insertAttribute("PS_COMMENT", CHAR_TYPE, 200);

	return dictID;
}
/*
tableOption.cpp
*/
/*
select.cpp
*/

int SPJ::tableScanEqualSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* value){
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
		int mapNo =bufOpt.reqPage(head, pageNo);
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

int SPJ::tableScanCountSelector(struct dbSysHead *head, int dictID){
	Relation rl = head->data_dict[dictID];
	int fid = rl.fileID;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[fid].filePageNum;

	int tmp_table_dictID = createTmpTable(head, rl);
	if (tmp_table_dictID < 0){
		printf("������ʱ��ʧ�ܣ�\n");
		return -1;
	} 
	int count = 0;
	//���������ϵ��ÿ��Ԫ��
	for (int i = 0; i < pageNum; i++){
		int mapNo =bufOpt.reqPage(head, pageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
		/*for (int j = 0; j < ph.curRecordNum; j++) {

		}*/
		count += ph.curRecordNum;
	} 
	char str1[25];
	itoa(count, str1, 10);
	insertOneRecord(head, tmp_table_dictID, str1);
	return tmp_table_dictID;
}

int SPJ::tableScanRangeSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* min, char* max){
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
		int mapNo =bufOpt.reqPage(head, pageNo);
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
/*
select.cpp
*/
/*
projection.cpp
*/
int SPJ::projection(struct dbSysHead *head, int dictID, char* attribute_name){
	Relation rl = head->data_dict[dictID];
	int fid = rl.fileID;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[fid].filePageNum;

	//�õ�ͶӰ��ָ������������
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
		printf("������ʱ��ʧ�ܣ�\n");
		return -1;
	}
	for (int i = 0; i < pageNum; i++){
		int mapNo =bufOpt.reqPage(head, pageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
		//�����ҳ�е�ÿһ����¼
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
					//exit(0);
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

int SPJ::getValueByAttrID(char *str, int index, char *result){
	int length = strlen(str);
	memset(result, 0, length);
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
/*
projection.cpp
*/
/*
join.cpp
*/
int SPJ::nestedLoopJoin(struct dbSysHead *head, int employee_dictID, int department_dictID){
	Relation emp = head->data_dict[employee_dictID];
	Relation dept = head->data_dict[department_dictID];

	//�ҵ�Ҫ�����ӵĹ�������(���±�)
	int emp_pub_attr = 0, dept_pub_attr = 0;
	bool isFound = false;
	for (emp_pub_attr = 0; emp_pub_attr < emp.attributeNum; emp_pub_attr++) {
		for (dept_pub_attr = 0; dept_pub_attr < dept.attributeNum; dept_pub_attr++) {
			if (emp.atb[emp_pub_attr].isSameAttribute(dept.atb[dept_pub_attr])){
				isFound = true;
				break;
			}
		}
		if (isFound)
			break;
	}
	if (isFound == false) {
		printf("����ʧ�ܣ���%s�ͱ�%sû�й������ԣ�\n", emp.relationName, dept.relationName);
		return -1;
	}
	int emp_fid = emp.fileID;
	long emp_pageNo = head->desc.fileDesc[emp_fid].fileFirstPageNo;
	long emp_pageNum = head->desc.fileDesc[emp_fid].filePageNum;

	int dept_fid = dept.fileID;
	long dept_pageNo = head->desc.fileDesc[dept_fid].fileFirstPageNo;
	long dept_pageNum = head->desc.fileDesc[dept_fid].filePageNum;


	int tmp_table_dictID = createTmpTable2(head, emp, dept, emp_pub_attr, dept_pub_attr);
	if (tmp_table_dictID < 0){
		printf("������ʱ��ʧ�ܣ�\n");
		return -1;
	}
	Relation *tmp = &(head->data_dict[tmp_table_dictID]);

	//���ѭ������С�ģ���ʱ�ٶ�departmentС
	int *buffID = (int*)malloc(dept_pageNum);
	int m = SIZE_BUFF - 1;
	int outer = dept_pageNum / m + 1;
	for (int x = 0; x < outer; x++){
		for (int yy = 0; yy < m && yy < dept_pageNum; yy++) {
			int mapNo =bufOpt.reqPage(head, dept_pageNo);
			struct pageHead ph;
			memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
			head->buff.map[mapNo].isPin = true; //��dept����Щҳ��pin�ڻ�������
			buffID[yy] = mapNo;
			if (ph.nextPageNo < 0)
				break;
			else
				dept_pageNo = ph.nextPageNo;
		}

		for (int z = 0; z < emp_pageNum; z++) {
			int emp_mapNo =bufOpt.reqPage(head, emp_pageNo);
			struct pageHead emp_ph;
			memcpy(&emp_ph, head->buff.data[emp_mapNo], SIZE_PAGEHEAD);
			for (int k = 0; k < emp_ph.curRecordNum; k++) {
				char *emp_record = (char*)malloc(emp.recordLength);
				//memset(emp_record, 0, emp.recordLength);
				getNextRecord(head, emp_mapNo, k, emp_record);
				char *emp_value = (char*)malloc(emp.recordLength);
				//memset(emp_value, 0, strlen(emp_record));
				int pe = getValueByAttrID(emp_record, emp_pub_attr, emp_value);//�õ����������ϵ�ֵ
				if (pe < 0){
					printf("bug in join.cpp\n");

				}
				for (int y = 0; y < m && y < dept_pageNum; y++){
					int dept_mapNo = buffID[y];
					struct pageHead dept_ph;
					memcpy(&dept_ph, head->buff.data[dept_mapNo], SIZE_PAGEHEAD);
					for (int i = 0; i < dept_ph.curRecordNum; i++) {
						char *dept_record = (char*)malloc(dept.recordLength);
						//memset(dept_record, 0, dept.recordLength);
						getNextRecord(head, dept_mapNo, i, dept_record);
						char *dept_value = (char*)malloc(dept.recordLength);
						//memset(dept_value, 0, strlen(dept_record));
						int pd = getValueByAttrID(dept_record, dept_pub_attr, dept_value);//�õ����������ϵ�ֵ
						if (pd < 0){
							printf("bug in join.cpp\n");

						}

						if (strcmp(emp_value, dept_value) == 0){
							char *result = (char*)malloc(tmp->recordLength);
							memset(result, 0, tmp->recordLength);
							//����������Ԫ�飬������ʱ����
							strcpy(result, emp_record);
							strcat(result, "|");
							strncat(result, dept_record, pd);
							strcat(result, dept_record + pd + strlen(dept_value) + 1);
							insertOneRecord(head, tmp_table_dictID, result);
						}
						free(dept_record);
						free(dept_value);
					}
				}
				free(emp_record);
				free(emp_value);
			}
			if (emp_ph.nextPageNo < 0)
				break;
			else
				emp_pageNo = emp_ph.nextPageNo;
		}
	}  
	return tmp_table_dictID;
} 
int SPJ::nestedLoopJoinByConds(struct dbSysHead *head, int birthday_dictID, int department_dictID, char* attribute_name, char* value, int like){
	Relation birth = head->data_dict[birthday_dictID];
	Relation dept = head->data_dict[department_dictID];

	//�ҵ�Ҫ�����ӵĹ�������(���±�)
	int birth_pub_attr = 0, dept_pub_attr = 0;
	bool isFound = false;
	for (birth_pub_attr = 0; birth_pub_attr < birth.attributeNum; birth_pub_attr++) {
		for (dept_pub_attr = 0; dept_pub_attr < dept.attributeNum; dept_pub_attr++) {
			if (birth.atb[birth_pub_attr].isSameAttribute(dept.atb[dept_pub_attr])){
				isFound = true;
				break;
			}
		}
		if (isFound)
			break;
	}
	if (isFound == false) {
		printf("����ʧ�ܣ���%s�ͱ�%sû�й������ԣ�\n", birth.relationName, dept.relationName);
		return -1;
	}
	int birth_fid = birth.fileID;
	long birth_pageNo = head->desc.fileDesc[birth_fid].fileFirstPageNo;
	long birth_pageNum = head->desc.fileDesc[birth_fid].filePageNum;

	int dept_fid = dept.fileID;
	long dept_pageNo = head->desc.fileDesc[dept_fid].fileFirstPageNo;
	long dept_pageNum = head->desc.fileDesc[dept_fid].filePageNum;


	int tmp_table_dictID = createTmpTable2(head, birth, dept, birth_pub_attr, dept_pub_attr);
	if (tmp_table_dictID < 0){
		printf("������ʱ��ʧ�ܣ�\n");
		return -1;
	}
	Relation *tmp = &(head->data_dict[tmp_table_dictID]);

	//���ѭ������С�ģ���ʱ�ٶ�departmentС
	int *buffID = (int*)malloc(dept_pageNum);
	int m = SIZE_BUFF - 1;
	int outer = dept_pageNum / m + 1;
	for (int x = 0; x < outer; x++){
		for (int y = 0; y < m && y < dept_pageNum; y++) {
			int mapNo =bufOpt.reqPage(head, dept_pageNo);
			struct pageHead ph;
			memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
			head->buff.map[mapNo].isPin = true; //��dept����Щҳ��pin�ڻ�������
			buffID[y] = mapNo;
			if (ph.nextPageNo < 0)
				break;
			else
				dept_pageNo = ph.nextPageNo;
		}
		// ��������
		int attr_index = birth.getAttributeIndexByName(attribute_name);

		for (int z = 0; z < birth_pageNum; z++) {
			int birth_mapNo =bufOpt.reqPage(head, birth_pageNo);
			struct pageHead birth_ph;
			memcpy(&birth_ph, head->buff.data[birth_mapNo], SIZE_PAGEHEAD);
			for (int k = 0; k < birth_ph.curRecordNum; k++) {
				char *birth_record = (char*)malloc(birth.recordLength);
				getNextRecord(head, birth_mapNo, k, birth_record);
				char *birth_value = (char*)malloc(birth.recordLength);
				int pe = getValueByAttrID(birth_record, birth_pub_attr, birth_value);//�õ����������ϵ�ֵ
				if (pe < 0){
					printf("bug in join.cpp\n");

				}

				if (attr_index < 0){
					printf("��ϵ��%s��û����Ϊ%s�����ԣ�\n", birth.relationName, attribute_name);
					return -1;
				}
				else{
					char *val = (char*)malloc(strlen(birth_record));
					memset(val, 0, strlen(birth_record));
					int s = getValueByAttrID(birth_record, attr_index, val);
					// [�����ж�](https://zhidao.baidu.com/question/438950092.html)
					bool flag = false;
					if (like == 0){

					}
					else if (like == 1){
						if (strstr(val, value))
							flag = true;
					}
					else if (like == 2){

					}
					else if (like == 3){
						string s = val;
						string::size_type idx = s.find(value);
						if (idx == string::npos)
							flag = true;
					}
					if (flag){
						for (int y = 0; y < m && y < dept_pageNum; y++){
							int dept_mapNo = buffID[y];
							struct pageHead dept_ph;
							memcpy(&dept_ph, head->buff.data[dept_mapNo], SIZE_PAGEHEAD);
							for (int i = 0; i < dept_ph.curRecordNum; i++) {
								char *dept_record = (char*)malloc(dept.recordLength);
								getNextRecord(head, dept_mapNo, i, dept_record);
								char *dept_value = (char*)malloc(dept.recordLength);
								int pd = getValueByAttrID(dept_record, dept_pub_attr, dept_value);//�õ����������ϵ�ֵ
								if (pd < 0){
									printf("bug in join.cpp\n");

								}

								if (strcmp(birth_value, dept_value) == 0){
									char *result = (char*)malloc(tmp->recordLength);
									memset(result, 0, tmp->recordLength);
									//����������Ԫ�飬������ʱ����
									strcpy(result, birth_record);
									strcat(result, "|");
									// bug, ����-1
									strncat(result, dept_record, pd - 1);
									insertOneRecord(head, tmp_table_dictID, result);
								}
							}
						}
					}
				}

			}
			if (birth_ph.nextPageNo < 0)
				break;
			else
				birth_pageNo = birth_ph.nextPageNo;
		}
	}

	return tmp_table_dictID;
}  
int SPJ::nestedLoopJoinByThree(struct dbSysHead *head, int temp_dictID, int department_dictID, int birthday_dictID){
	Relation temp = head->data_dict[temp_dictID]; // 5
	Relation birth = head->data_dict[birthday_dictID];// 10

	//�ҵ�Ҫ�����ӵĹ�������(���±�)
	int temp_pub_attr = 0, birth_pub_attr = 0;
	bool isFound = false;
	for (temp_pub_attr = 0; temp_pub_attr < temp.attributeNum; temp_pub_attr++) {
		for (birth_pub_attr = 0; birth_pub_attr < birth.attributeNum; birth_pub_attr++) {
			if (temp.atb[temp_pub_attr].isSameAttribute(birth.atb[birth_pub_attr])){
				isFound = true;
				break;
			}
		}
		if (isFound)
			break;
	}
	if (isFound == false) {
		printf("����ʧ�ܣ���%s�ͱ�%sû�й������ԣ�\n", temp.relationName, birth.relationName);
		return -1;
	}
	int temp_fid = temp.fileID;
	long temp_pageNo = head->desc.fileDesc[temp_fid].fileFirstPageNo;
	long temp_pageNum = head->desc.fileDesc[temp_fid].filePageNum;

	int birth_fid = birth.fileID;
	long birth_pageNo = head->desc.fileDesc[birth_fid].fileFirstPageNo;
	long birth_pageNum = head->desc.fileDesc[birth_fid].filePageNum;


	int tmp_table_dictID = createTmpTable2(head, temp, birth, temp_pub_attr, birth_pub_attr);
	if (tmp_table_dictID < 0){
		printf("������ʱ��ʧ�ܣ�\n");
		return -1;
	}
	Relation *tmp = &(head->data_dict[tmp_table_dictID]);

	//���ѭ������С�ģ���ʱ�ٶ�departmentС
	int *buffID = (int*)malloc(birth_pageNum);
	int m = SIZE_BUFF - 1;
	int outer = birth_pageNum / m + 1;
	for (int x = 0; x < outer; x++){
		for (int y = 0; y < m && y < birth_pageNum; y++) {
			int mapNo =bufOpt.reqPage(head, birth_pageNo);
			struct pageHead ph;
			memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
			head->buff.map[mapNo].isPin = true; //��birth����Щҳ��pin�ڻ�������
			buffID[y] = mapNo;
			if (ph.nextPageNo < 0)
				break;
			else
				birth_pageNo = ph.nextPageNo;
		}

		for (int z = 0; z < temp_pageNum; z++) {
			int temp_mapNo =bufOpt.reqPage(head, temp_pageNo);
			struct pageHead temp_ph;
			memcpy(&temp_ph, head->buff.data[temp_mapNo], SIZE_PAGEHEAD);
			for (int k = 0; k < temp_ph.curRecordNum; k++) {
				char *temp_record = (char*)malloc(temp.recordLength);
				getNextRecord(head, temp_mapNo, k, temp_record);
				char *temp_value = (char*)malloc(temp.recordLength);
				int pe = getValueByAttrID(temp_record, temp_pub_attr, temp_value);//�õ����������ϵ�ֵ
				if (pe < 0){
					printf("bug in join.cpp\n");

				}
				for (int y = 0; y < m && y < birth_pageNum; y++){
					int birth_mapNo = buffID[y];
					struct pageHead birth_ph;
					memcpy(&birth_ph, head->buff.data[birth_mapNo], SIZE_PAGEHEAD);
					for (int i = 0; i < birth_ph.curRecordNum; i++) {
						char *birth_record = (char*)malloc(birth.recordLength);
						getNextRecord(head, birth_mapNo, i, birth_record);
						char *birth_value = (char*)malloc(birth.recordLength);
						int pd = getValueByAttrID(birth_record, birth_pub_attr, birth_value);//�õ����������ϵ�ֵ
						if (pd < 0){
							printf("bug in join.cpp\n");

						}

						if (strcmp(temp_value, birth_value) == 0){
							char *result = (char*)malloc(tmp->recordLength);
							memset(result, 0, tmp->recordLength);
							//����������Ԫ�飬������ʱ����
							strcpy(result, temp_record);
							strcat(result, "|");
							strncat(result, birth_record, pd);
							strcat(result, birth_record + pd + strlen(birth_value) + 1);
							insertOneRecord(head, tmp_table_dictID, result);
						}
					}
				}

			}
			if (temp_ph.nextPageNo < 0)
				break;
			else
				temp_pageNo = temp_ph.nextPageNo;
		}
	}

	return tmp_table_dictID;
} 
int SPJ::SortJoin(struct dbSysHead *head, int employee_dictID, int department_dictID) {
	Relation emp = head->data_dict[employee_dictID];
	Relation dept = head->data_dict[department_dictID];

	//�ҵ�Ҫ�����ӵĹ�������(���±�)
	int emp_pub_attr = 0, dept_pub_attr = 0;
	bool isFound = false;
	for (emp_pub_attr = 0; emp_pub_attr < emp.attributeNum; emp_pub_attr++) {
		for (dept_pub_attr = 0; dept_pub_attr < dept.attributeNum; dept_pub_attr++) {
			if (emp.atb[emp_pub_attr].isSameAttribute(dept.atb[dept_pub_attr])){
				isFound = true;
				break;
			}
		}
		if (isFound)
			break;
	}
	if (isFound == false) {
		printf("����ʧ�ܣ���%s�ͱ�%sû�й������ԣ�\n", emp.relationName, dept.relationName);
		return -1;
	}
	int tmp_table_dictID = createTmpTable2(head, emp, dept, emp_pub_attr, dept_pub_attr);
	if (tmp_table_dictID < 0){
		printf("������ʱ��ʧ�ܣ�\n");
		return -1;
	}

	//����������������ݴ浽��ʱ�ļ���
	int tmp_table_emp = createTmpTableAfterSort(head, emp, emp_pub_attr);
	int tmp_table_dept = createTmpTableAfterSort(head, dept, dept_pub_attr);

	//just for test!
	//readFile(head, tmp_table_emp);
	//readFile(head, tmp_table_dept);

	Relation *tmp = &(head->data_dict[tmp_table_dictID]);
	Relation *tmp_emp = &(head->data_dict[tmp_table_emp]);
	Relation *tmp_dept = &(head->data_dict[tmp_table_dept]);

	long pageNo_emp = head->desc.fileDesc[tmp_emp->fileID].fileFirstPageNo;
	long pageNum_emp = head->desc.fileDesc[tmp_emp->fileID].filePageNum;

	long pageNo_dept = head->desc.fileDesc[tmp_dept->fileID].fileFirstPageNo;
	long pageNum_dept = head->desc.fileDesc[tmp_dept->fileID].filePageNum;

	for (int j = 0; j < pageNum_dept; j++) {
		int mapNo_dept =bufOpt.reqPage(head, pageNo_dept);
		struct pageHead ph_dept;
		memcpy(&ph_dept, head->buff.data[mapNo_dept], SIZE_PAGEHEAD);
		for (int i = 0; i < ph_dept.curRecordNum; i++) {
			char *record_dept = (char*)malloc(tmp_dept->recordLength);
			getNextRecord(head, mapNo_dept, i, record_dept);
			char *val_dept = (char*)malloc(strlen(record_dept));
			int pd = getValueByAttrID(record_dept, dept_pub_attr, val_dept);

			for (int x = 0; x < pageNum_emp; x++) {
				int mapNo_emp =bufOpt.reqPage(head, pageNo_emp);
				struct pageHead ph_emp;
				memcpy(&ph_emp, head->buff.data[mapNo_emp], SIZE_PAGEHEAD);
				bool flag = false;
				for (int y = 0; y < ph_emp.curRecordNum; y++){
					char *record_emp = (char*)malloc(tmp_emp->recordLength);
					getNextRecord(head, mapNo_emp, y, record_emp);
					char* val_emp = (char*)malloc(strlen(record_emp));
					getValueByAttrID(record_emp, emp_pub_attr, val_emp);
					if (strcmp(val_emp, val_dept) == 0){
						char *result = (char*)malloc(tmp->recordLength);
						memset(result, 0, tmp->recordLength);
						//����������Ԫ�飬������ʱ����
						strcpy(result, record_emp);
						strcat(result, "|");
						strncat(result, record_dept, pd);
						strcat(result, record_dept + pd + strlen(val_dept) + 1);
						insertOneRecord(head, tmp_table_dictID, result);
					}
					else if (strcmp(val_emp, val_dept) > 0){
						flag = true;
						break;
					}
					else {
						continue;
					}
				}
				if (flag)
					break;
				if (ph_emp.nextPageNo < 0)
					break;
				else
					pageNo_emp = ph_emp.nextPageNo;
			}

		}
		if (ph_dept.nextPageNo < 0)
			break;
		else
			pageNo_dept = ph_dept.nextPageNo;

	}
	return tmp_table_dictID;
} 
int SPJ::HashJoin(struct dbSysHead *head, int employee_dictID, int department_dictID){
	Relation emp = head->data_dict[employee_dictID];
	Relation dept = head->data_dict[department_dictID];

	//�ҵ�Ҫ�����ӵĹ�������(���±�)
	int emp_pub_attr = 0, dept_pub_attr = 0;
	bool isFound = false;
	for (emp_pub_attr = 0; emp_pub_attr < emp.attributeNum; emp_pub_attr++) {
		for (dept_pub_attr = 0; dept_pub_attr < dept.attributeNum; dept_pub_attr++) {
			if (emp.atb[emp_pub_attr].isSameAttribute(dept.atb[dept_pub_attr])){
				isFound = true;
				break;
			}
		}
		if (isFound)
			break;
	}
	if (isFound == false) {
		printf("����ʧ�ܣ���%s�ͱ�%sû�й������ԣ�\n", emp.relationName, dept.relationName);
		return -1;
	}
	int tmp_table_dictID = createTmpTable2(head, emp, dept, emp_pub_attr, dept_pub_attr);
	if (tmp_table_dictID < 0){
		printf("������ʱ��ʧ�ܣ�\n");
		return -1;
	}
	Relation *tmp = &(head->data_dict[tmp_table_dictID]);
	multimap<int, long> m_emp[BUCKET_NUM];
	multimap<int, long> m_dept[BUCKET_NUM];
	//    int tmp_table_emp = createTmpTableAfterHash(head, emp, emp_pub_attr,m_emp);
	//    int tmp_table_dept = createTmpTableAfterHash(head, dept, dept_pub_attr, m_dept);
	HashRelation(head, emp, emp_pub_attr, m_emp);
	HashRelation(head, dept, dept_pub_attr, m_dept);

	for (int i = 0; i < BUCKET_NUM; i++) {
		map<int, long>::iterator it_emp, it_dept;
		for (it_dept = m_dept[i].begin(); it_dept != m_dept[i].end(); it_dept++) {
			for (it_emp = m_emp[i].begin(); it_emp != m_emp[i].end(); it_emp++){
				if (it_emp->first == it_dept->first){
					char* record_emp = (char*)malloc(emp.recordLength);
					char* record_dept = (char*)malloc(dept.recordLength);
					queryRecordByLogicID(head, it_emp->second, record_emp);
					queryRecordByLogicID(head, it_dept->second, record_dept);

					char* val_dept = new char[strlen(record_dept)];
					int pd = getValueByAttrID(record_dept, dept_pub_attr, val_dept);

					char *result = (char*)malloc(tmp->recordLength);
					memset(result, 0, tmp->recordLength);
					//����������Ԫ�飬������ʱ����
					strcpy(result, record_emp);
					strcat(result, "|");
					strncat(result, record_dept, pd);
					strcat(result, record_dept + pd + strlen(val_dept) + 1);
					insertOneRecord(head, tmp_table_dictID, result);
				}
				else if (it_emp->first > it_dept->first) {
					break;
				}
				else {
					continue;
				}
			}
		}
	}
	return tmp_table_dictID;
}
/*
join.cpp
*/
/*
tmpTable.cpp
*/
int SPJ::createTmpTable(struct dbSysHead *head, Relation original_rl){
	int fid = fileOpt.createFile(head, TMP_TABLE, 1);
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
int SPJ::createTmpTable2(struct dbSysHead *head, Relation r1, Relation r2, int r1_pub_attr, int r2_pub_attr){
	int fid = fileOpt.createFile(head, TMP_TABLE, 1);
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
int SPJ::createTmpTable3(struct dbSysHead *head, Relation r1, Relation r2, Relation r3, int r1_pub_attr, int r2_pub_attr, int r3_pub_attr){
	int fid = fileOpt.createFile(head, TMP_TABLE, 1);
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0 && head->data_dict[i].attributeNum == 0){
			//�Ȱ�r1��ȫ���� head->data_dict[i]
			head->data_dict[i] = r1;
			head->data_dict[i].fileID = fid;
			strcat(head->data_dict[i].relationName, "_");
			strcat(head->data_dict[i].relationName, r2.relationName);
			strcat(head->data_dict[i].relationName, "_");
			strcat(head->data_dict[i].relationName, r3.relationName);
			strcat(head->data_dict[i].relationName, "_tmp");
			head->data_dict[i].attributeNum = r1.attributeNum + r2.attributeNum + r3.attributeNum - 1;
			int j = r1.attributeNum;
			for (int k = 0; k < r2.attributeNum; k++) {
				if (k != r2_pub_attr){
					head->data_dict[i].atb[j] = r2.atb[k];
					head->data_dict[i].isIndexed[j] = r2.isIndexed[k];
					head->data_dict[i].isOrdered[j] = r2.isOrdered[k];
					j++;
				}
			}
			for (int k = 0; k < r3.attributeNum; k++) {
				if (k != r3_pub_attr){
					head->data_dict[i].atb[j] = r3.atb[k];
					head->data_dict[i].isIndexed[j] = r3.isIndexed[k];
					head->data_dict[i].isOrdered[j] = r3.isOrdered[k];
					j++;
				}
			}

			head->data_dict[i].recordNum = 0;
			head->data_dict[i].recordLength = r1.recordLength + r2.recordLength + r3.recordLength - r1.atb[r1_pub_attr].getLength() - r2.atb[r2_pub_attr].getLength();

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
int SPJ::createTmpTableAfterSort(struct dbSysHead *head, Relation rl, int pub_attr){
	int fid = fileOpt.createFile(head, TMP_TABLE, 1);
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
		int mapNo =bufOpt.reqPage(head, pageNo);
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

void SPJ::HashRelation(struct dbSysHead *head, Relation rl, int pub_attr, multimap<int, long> *m) {
	int rl_fid = rl.fileID;
	long pageNo = head->desc.fileDesc[rl_fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[rl_fid].filePageNum;

	for (int i = 0; i < pageNum; i++) {
		int mapNo = bufOpt.reqPage(head, pageNo);
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
int SPJ::insertOneRecord(struct dbSysHead *head, int tmp_table_dictID, char* record){
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
		mapNo = bufOpt.reqPage(head, curPageNo);
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
		struct pageHead ph = fileOpt.extendFile(head, fid, &PHead);

		curOffset.logicID = -1;
		curOffset.recordID = 0;
		curOffset.offset = length;
		curOffset.isDeleted = false;

		curRecordPos = SIZE_PER_PAGE - length;
		curOffsetPos = SIZE_PAGEHEAD;

		ph.curRecordNum = 1;
		ph.freeSpace = ph.freeSpace - SIZE_OFFSET - length;

		//������ҳ��ҳͷ
		mapNo =bufOpt.reqPage(head, ph.pageNo);
		memcpy(head->buff.data[mapNo], &ph, SIZE_PAGEHEAD);

	}
	//����ƫ��������¼ͷ���ͼ�¼
	memcpy(head->buff.data[mapNo] + curOffsetPos, &curOffset, SIZE_OFFSET);
	memcpy(head->buff.data[mapNo] + curRecordPos, record, length);
	head->buff.map[mapNo].isEdited = true;
	head->data_dict[tmp_table_dictID].recordNum++;
	return mapNo;
}
/*
tmpTable.cpp
*/