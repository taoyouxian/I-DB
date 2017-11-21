#include "dbHead.h"
#include "common.h"

//�������������������ֵ��е��±�
int createTable_employee(struct dbSysHead *head) {
	int fid = createFile(head, USER_FILE, 1);
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
int createTable_employee(struct dbSysHead *head, string sql) {
	int fid = createFile(head, USER_FILE, 1);
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
int createTable_department(struct dbSysHead *head) {
	int fid = createFile(head, USER_FILE, 1);
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
int createTable_birthday(struct dbSysHead *head) {
	int fid = createFile(head, USER_FILE, 1);
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
int createTable_supplier(struct dbSysHead *head) {
	int fid = createFile(head, USER_FILE, 1);
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
int createTable_partsupp(struct dbSysHead *head) {
	int fid = createFile(head, USER_FILE, 1);
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