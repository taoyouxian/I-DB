#include "dbHead.h"
#include "common.h"

//创建表，返回其在数据字典中的下标
int createTable_employee(struct dbSysHead *head) {
	int fid = createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("Failed to create user file.");
		return  -1;
	}
	else
		printf("Create user file %d successfully.\n\n", fid);

	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件！\n", fid);
		return -1;
	}
	//在数据字典中找一个空位
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("当前数据库中已存在过多的关系，无法创建新关系！\n");
		return  -1;
	}
	/* todo:

	char* table_pattern;
	printf("本系统的基本语法可参考SQL。\n");
	printf("以创建表的语句为例：\n");
	printf("create table employee(rid INT, did INT, rname VARCHAR(20), age INT, PRIMARY KEY(rid));\n");
	printf("该语句表示创建了一个名为employee的表\n");
	printf("共包含三个字段，分别是int类型的rid和age，以及varchar类型的name\n");
	printf("其中rid为该表的主键\n");
	printf("下面，请创建您的表");
	scanf("%s", table_pattern);

	* 写一段parser解析table_pattern（create语句）
	* 得到：relationName,attributeName和attributeType
	* 利用attributeName和attributeType构建多个Attribute变量，进而构建属性表
	* 再构建Relation变量
	* 目前先手动构建relation
	*/
	head->data_dict[dictID].initRelation(head, fid, "employee");
	head->data_dict[dictID].insertAttribute("rid", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("did", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("age", INT_TYPE, 4);
	head->data_dict[dictID].insertAttribute("rname", CHAR_TYPE, 20);

	return dictID;
}

//创建表，返回其在数据字典中的下标
int createTable_employee(struct dbSysHead *head, string sql) {
	int fid = createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("Failed to create user file.");
		return  -1;
	}
	else
		printf("Create user file %d successfully.\n\n", fid);

	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件！\n", fid);
		return -1;
	}
	//在数据字典中找一个空位
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("当前数据库中已存在过多的关系，无法创建新关系！\n");
		return  -1;
	}
	/* todo:

	char* table_pattern;
	printf("本系统的基本语法可参考SQL。\n");
	printf("以创建表的语句为例：\n");
	printf("create table employee(rid INT, did INT, rname VARCHAR(20), age INT, PRIMARY KEY(rid));\n");
	printf("该语句表示创建了一个名为employee的表\n");
	printf("共包含三个字段，分别是int类型的rid和age，以及varchar类型的name\n");
	printf("其中rid为该表的主键\n");
	printf("下面，请创建您的表");
	scanf("%s", table_pattern);

	* 写一段parser解析table_pattern（create语句）
	* 得到：relationName,attributeName和attributeType
	* 利用attributeName和attributeType构建多个Attribute变量，进而构建属性表
	* 再构建Relation变量
	* 目前先手动构建relation
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

//创建表，返回其在数据字典中的下标
int createTable_department(struct dbSysHead *head) {
	int fid = createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("创建用户文件失败！");
		return  -1;
	}
	else
		printf("Create user file %d successfully.\n\n", fid);

	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件！\n", fid);
		return -1;
	}
	//在数据字典中找一个空位
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("当前数据库中已存在过多的关系，无法创建新关系！\n");
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

//创建表，返回其在数据字典中的下标
int createTable_birthday(struct dbSysHead *head) {
	int fid = createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("创建用户文件失败！");
		return  -1;
	}
	else
		printf("Create user file %d successfully.\n\n", fid);

	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件！\n", fid);
		return -1;
	}
	//在数据字典中找一个空位
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("当前数据库中已存在过多的关系，无法创建新关系！\n");
		return  -1;
	}

	head->data_dict[dictID].initRelation(head, fid, "birthday");
	head->data_dict[dictID].insertAttribute("idcard", CHAR_TYPE, 18);
	head->data_dict[dictID].insertAttribute("birthtime", CHAR_TYPE, 15);
	head->data_dict[dictID].insertAttribute("addressid", INT_TYPE, 2);

	return dictID;
}

//创建表，返回其在数据字典中的下标
int createTable_supplier(struct dbSysHead *head) {
	int fid = createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("创建用户文件失败！");
		return  -1;
	}
	else
		printf("Create user file %d successfully.\n\n", fid);

	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件！\n", fid);
		return -1;
	}
	//在数据字典中找一个空位
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("当前数据库中已存在过多的关系，无法创建新关系！\n");
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

//创建表，返回其在数据字典中的下标
int createTable_partsupp(struct dbSysHead *head) {
	int fid = createFile(head, USER_FILE, 1);
	if (fid < 0){
		printf("创建用户文件失败！");
		return  -1;
	}
	else
		printf("Create user file %d successfully.\n\n", fid);

	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件！\n", fid);
		return -1;
	}
	//在数据字典中找一个空位
	int dictID = -1;
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID < 0){
			dictID = i;
			break;
		}
	}
	if (dictID < 0) {
		printf("当前数据库中已存在过多的关系，无法创建新关系！\n");
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