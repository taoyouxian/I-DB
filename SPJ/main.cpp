#include "dbHead.h"
#include <sstream>
void changeTime(char *time);

int main() {
	welcome();
	char filename[100];
	strcpy(filename, PATH);
	strcat(filename, "database.mat\0");
	// remove("database.txt");

	struct dbSysHead head;
	init_database(&head, filename);

	show_SysDesc(&head);

	if (head.desc.fid_MapTable < 0){
		int MapTable_fid = createFile(&head, MAP_FILE, 1);
		head.desc.fid_MapTable = MapTable_fid;
		printf("Create mapping table %d successfully.\n\n", MapTable_fid);
	}

	if (head.desc.fid_DataDictionary < 0) {
		int dataDict_fid = createFile(&head, DATA_DICTIONARY, 1);;
		head.desc.fid_DataDictionary = dataDict_fid;
		printf("Create data dictionary file %d successfully.\n\n", dataDict_fid);
	}

	string sql = "create table employee(rid INT(11), did INT, rname VARCHAR(20), age INT, PRIMARY KEY(rid))";
	//int employee_dictID = createTable_employee(&head);
	int employee_dictID = createTable_employee(&head, sql);
	if (employee_dictID < 0){
		printf("Failed to create table employee.\n");
		exit(0);
	}

	for (int i = 0; i < 200; i++) {
		int did = rand() % 10;
		int age = rand() % 40 + 20;
		struct employee emp = { i, did, age, "abc" };

		char str[1000];
		sprintf(str, "%d", emp.rid);
		strcat(str, "|");

		char tmp[100];
		sprintf(tmp, "%d", emp.did);
		strcat(str, tmp);
		strcat(str, "|");

		sprintf(tmp, "%d", emp.age);
		strcat(str, tmp);
		strcat(str, "|");

		strcat(str, emp.rname);
		sprintf(tmp, "%d", i / 10);
		strcat(str, tmp);
		// 传入的字符串形如"1|5|30|abc"
		writeFile(&head, employee_dictID, str /*, rh*/);
	}

	show_FileDesc(&head, employee_dictID);
	readFile(&head, employee_dictID);

	show_MapTableFile(&head, head.desc.fid_MapTable);

	// 测试用逻辑号去找某条记录
	char des[1000];
	for (int i = 0; i < 10; i++) {
		if (queryRecordByLogicID(&head, i * 15, des) < 0)
			continue;
		printf("LogicID %d's content: %s\n", i * 15, des);
	}

	// 等值选择
	// 应该是输入select语句，解析出table_name，再去data_dict里面找到相应的relation
	// 再从输入中（where后面）得到属性名以及value
	// SQL: select * from employee where did = 6
	int s1 = tableScanEqualSelector(&head, employee_dictID, "did", "6");
	readFile(&head, s1);

	// 范围选择
	// SQL: select * from employee where age > 30 and age < 40􀋅
	int s2 = tableScanRangeSelector(&head, employee_dictID, "age", "30", "40");
	readFile(&head, s2);

	// 投影
	// 应该是输入“select 属性”语句，得到用户指定的属性名，再传入函数中
	// SQL: select rid, did from employee
	char* attribute_name = "rid|did";
	int p = projection(&head, employee_dictID, attribute_name);
	readFile(&head, p);

	//创建表department
	int department_dictID = createTable_department(&head);
	if (department_dictID < 0){
		printf("Create table department failed.\n");
		exit(0);
	}
	for (int i = 0; i < 10; i++) {
		int num = rand() % 20 + 5;
		struct department d = { i, i * 2, num, "manager", "34052119950103331" };
		char str[1000];
		sprintf(str, "%d", d.did);
		strcat(str, "|");

		char tmp[100];
		sprintf(tmp, "%d", d.manager_id);
		strcat(str, tmp);
		strcat(str, "|");

		sprintf(tmp, "%d", d.num);
		strcat(str, tmp);
		strcat(str, "|");

		strcat(str, d.dname);
		sprintf(tmp, "%d", i);
		strcat(str, tmp);
		strcat(str, "|");

		strcat(str, d.idcard);
		sprintf(tmp, "%d", i);
		strcat(str, tmp);
		//传入的字符串形如”1|5|30|defg{i}“
		writeFile(&head, department_dictID, str);
	}
	show_FileDesc(&head, department_dictID);
	readFile(&head, department_dictID);

	// SQL: select * from employee e, department d where e.did = d.did􀋅
	// 嵌套循环连接（在公共属性上的）
	int j1 = nestedLoopJoin(&head, employee_dictID, department_dictID);
	readFile(&head, j1);
	// 基于排序的等值连接
	int j2 = SortJoin(&head, employee_dictID, department_dictID);
	readFile(&head, j2);
	// 基于散列的等值连接
	int j3 = HashJoin(&head, employee_dictID, department_dictID);
	readFile(&head, j3);

	// 创建表birthday
	int birthday_dictID = createTable_birthday(&head);
	if (birthday_dictID < 0){
		printf("Create table birthday failed.\n");
		exit(0);
	}
	for (int i = 0; i < 4; i++) {
		int num = rand() % 20 + 5;
		char address[4][11] = { "1880-10-10", "1990-10-12", "1989-10-2", "1989-10-10" };

		struct birthday b = { "34052119950103331", "", i };
		strcpy(b.birthtime, address[i]);

		char str[1000];
		sprintf(str, "%s", b.idcard);

		char tmp[100];
		sprintf(tmp, "%d", i);
		strcat(str, tmp);
		strcat(str, "|");

		strcat(str, b.birthtime);
		strcat(str, "|");
		sprintf(tmp, "%d", i);
		strcat(str, tmp);

		// 传入的字符串形如"1|5|30|defg"
		writeFile(&head, birthday_dictID, str);
	}
	show_FileDesc(&head, birthday_dictID);
	readFile(&head, birthday_dictID);

	// 嵌套循环连接（在公共属性上的）,三表连接
	// SQL: select * from employee e, department d, birthday b where e.did = d.did and d.idcard = b.idcard
	int j4 = nestedLoopJoinByThree(&head, j1, department_dictID, birthday_dictID);
	readFile(&head, j4);

	// 不同数据类型的多条件等值连接（在公共属性上的）
	// SQL: select * from department d, birthday b where d.idcard = b.idcard and d.birthtime like '%19', 0-begin, 1-contains, 2-end
	int j5 = nestedLoopJoinByConds(&head, birthday_dictID, department_dictID, "birthtime", "89", 1);
	readFile(&head, j5);

	// 不同数据类型的多条件非等值连接（在公共属性上的）
	// SQL: select * from department d, birthday b where d.idcard = b.idcard and d.birthtime like '%19', 0-begin, 1-contains, 2-end, 3-not contains
	int j6 = nestedLoopJoinByConds(&head, birthday_dictID, department_dictID, "birthtime", "89", 3);
	readFile(&head, j6);

	// 导出csv
	writeFile(&head, j5);

	// 创建表supplier
	int supplier_dictID = createTable_supplier(&head);
	if (supplier_dictID < 0){
		printf("Create table supplier failed.\n");
		exit(0);
	}
	char str[1000];
	ifstream fin("H:\\SelfLearning\\SAI\\Course\\tinydbms\\SPJAlgorithm\\supplier.tbl");
	while (!fin.eof())
	{
		string line;
		getline(fin, line);
		strcpy(str, line.c_str());
		writeFile(&head, supplier_dictID, str);
	}
	show_FileDesc(&head, supplier_dictID);
	readFile(&head, supplier_dictID);

	// 创建表partsupp
	int partsupp_dictID = createTable_partsupp(&head);
	if (partsupp_dictID < 0){
		printf("Create table partsupp failed.\n");
		exit(0);
	}
	ifstream fin2("H:\\SelfLearning\\SAI\\Course\\tinydbms\\SPJAlgorithm\\partsupp.tbl");
	while (!fin2.eof())
	{
		string line;
		getline(fin2, line);
		strcpy(str, line.c_str());
		writeFile(&head, partsupp_dictID, str);
	}
	show_FileDesc(&head, partsupp_dictID);
	//readFile(&head, partsupp_dictID);

	int s3 = tableScanRangeSelector(&head, supplier_dictID, "S_superkey", "10", "20");
	readFile(&head, s3);

	close_database(&head);

	// 更改文件名
	char time[100];
	changeTime(time, ".mat");
	rename(filename, time);

	system("pause");
	return 0;
}
void initDB(struct dbSysHead *head, char *filename){
	init_database(head, filename);
	show_SysDesc(head);
}
void showInitInfo(struct dbSysHead head){
	if (head.desc.fid_MapTable < 0){
		int MapTable_fid = createFile(&head, MAP_FILE, 1);
		head.desc.fid_MapTable = MapTable_fid;
		printf("Create mapping table %d successfully.\n\n", MapTable_fid);
	}

	if (head.desc.fid_DataDictionary < 0) {
		int dataDict_fid = createFile(&head, DATA_DICTIONARY, 1);;
		head.desc.fid_DataDictionary = dataDict_fid;
		printf("Create data dictionary file %d successfully.\n\n", dataDict_fid);
	}
}
void showMain(){
	printf("\n");
	printf("    *********************************************************************************************\n");
	printf("    *      Welcome to TinyDB demonstration, choose your function:								*\n");
	printf("    *			1. Init tables(employee,department,birthday)									*\n");
	printf("    *           2. Insert datas to tables														*\n");
	printf("    *           3. Equivalent selection															*\n");
	printf("    *           4. Range selection																*\n");
	printf("    *           5. Projection																	*\n");
	printf("    *           6. Join(Nested loop/Sort-based/Hash-based)										*\n");
	printf("    *           7. Three tables connected														*\n");
	printf("    *           8. Multi-condition equivalent connection for different data types				*\n");
	printf("    *           9. Multi-conditional non-equivalentconnections of different data types			*\n");
	printf("    *           10. Show record bt logicID														*\n");
	printf("    *           11. Show mapping table file														*\n");
	printf("    *           12. Show system desc															*\n");
	printf("    *           13. Show file desc																*\n");
	printf("    *           14. Output(.csv) 																*\n");
	printf("    *           15. Backup(.mat) 																*\n");
	printf("    *           16. Main Window    																*\n");
	printf("    *           0. Exit																			*\n");
	printf("    *********************************************************************************************\n\n");
	printf("\nTinyDB> Please input your choise: ");
}
int initEmp(struct dbSysHead head){
	string sql = "create table employee(rid INT(11), did INT, rname VARCHAR(20), age INT, PRIMARY KEY(rid))";
	//int employee_dictID = createTable_employee(&head);
	int employee_dictID = createTable_employee(&head, sql);
	if (employee_dictID < 0){
		printf("Failed to create table employee.\n");
		exit(0);
	}
	return employee_dictID;
}
void genEmp(struct dbSysHead head, int employee_dictID){
	for (int i = 0; i < 200; i++) {
		int did = rand() % 10;
		int age = rand() % 40 + 20;
		struct employee emp = { i, did, age, "abc" };

		char str[1000];
		sprintf(str, "%d", emp.rid);
		strcat(str, "|");

		char tmp[100];
		sprintf(tmp, "%d", emp.did);
		strcat(str, tmp);
		strcat(str, "|");

		sprintf(tmp, "%d", emp.age);
		strcat(str, tmp);
		strcat(str, "|");

		strcat(str, emp.rname);
		sprintf(tmp, "%d", i / 10);
		strcat(str, tmp);
		// 传入的字符串形如"1|5|30|abc"
		writeFile(&head, employee_dictID, str /*, rh*/);
	}
}
void genDep(struct dbSysHead head, int department_dictID){
	for (int i = 0; i < 10; i++) {
		int num = rand() % 20 + 5;
		struct department d = { i, i * 2, num, "manager", "34052119950103331" };
		char str[1000];
		sprintf(str, "%d", d.did);
		strcat(str, "|");

		char tmp[100];
		sprintf(tmp, "%d", d.manager_id);
		strcat(str, tmp);
		strcat(str, "|");

		sprintf(tmp, "%d", d.num);
		strcat(str, tmp);
		strcat(str, "|");

		strcat(str, d.dname);
		sprintf(tmp, "%d", i);
		strcat(str, tmp);
		strcat(str, "|");

		strcat(str, d.idcard);
		sprintf(tmp, "%d", i);
		strcat(str, tmp);
		//传入的字符串形如”1|5|30|defg{i}“
		writeFile(&head, department_dictID, str);
	}
}
void genBirth(struct dbSysHead head, int birthday_dictID){
	for (int i = 0; i < 4; i++) {
		int num = rand() % 20 + 5;
		char address[4][11] = { "1880-10-10", "1990-10-12", "1989-10-2", "1989-10-10" };

		struct birthday b = { "34052119950103331", "", i };
		strcpy(b.birthtime, address[i]);

		char str[1000];
		sprintf(str, "%s", b.idcard);

		char tmp[100];
		sprintf(tmp, "%d", i);
		strcat(str, tmp);
		strcat(str, "|");

		strcat(str, b.birthtime);
		strcat(str, "|");
		sprintf(tmp, "%d", i);
		strcat(str, tmp);

		// 传入的字符串形如"1|5|30|defg"
		writeFile(&head, birthday_dictID, str);
	}
}
int initDep(struct dbSysHead head){
	//创建表department
	int department_dictID = createTable_department(&head);
	if (department_dictID < 0){
		printf("Create table department failed.\n");
		exit(0);
	}
	return department_dictID;
}
int initBirth(struct dbSysHead head){
	// 创建表birthday
	int birthday_dictID = createTable_birthday(&head);
	if (birthday_dictID < 0){
		printf("Create table birthday failed.\n");
		exit(0);
	}
	return birthday_dictID;
}
void operation(struct dbSysHead head, char *filename){
	// 选择操作
	int demonstrationCh;
	// 记录文件号
	int employee_dictID = 0, department_dictID = 0, birthday_dictID = 0, files[100], max_file = 0, fileID = 0;
	int s1 = 0, s2 = 0, p = 0, j1 = 0, j2 = 0, j3 = 0, j4 = 0, j5 = 0, j6 = 0, j7 = 0;
	scanf("%d", &demonstrationCh);
	if (demonstrationCh == 1){
		employee_dictID = initEmp(head);
		department_dictID = initDep(head);
		birthday_dictID = initBirth(head);
		max_file = birthday_dictID;
		printf("TinyDB> Notification: \n");
		printf("\tTable employee generated by SQL: create table employee(rid INT(11), did INT, rname VARCHAR(20), age INT, PRIMARY KEY(rid))\n");
		printf("\tTable department & birthday generated by default.\n");
		printf("\nTinyDB> Your choise: ");
	}
	else if (demonstrationCh == 2){
		printf("TinyDB> Default number of each table: 200, 10, 4\n");
		genEmp(head, employee_dictID);
		genDep(head, department_dictID);
		genBirth(head, birthday_dictID);
		printf("TinyDB> Generate successfully.\n");
		printf("TinyDB> Table info.\n");

		show_FileDesc(&head, employee_dictID);
		show_FileDesc(&head, department_dictID);
		show_FileDesc(&head, birthday_dictID);
		printf("\nTinyDB> Your choise: ");
	}
	else if (demonstrationCh == 3){
		printf("TinyDB> SQL: select * from employee where did = 6.\n");
		s1 = tableScanEqualSelector(&head, employee_dictID, "did", "6");
		//readFile(&head, s1);
		printf("\nTinyDB> Your choise: ");
		fileID = s1;
	}
	else if (demonstrationCh == 4){
		printf("TinyDB> SQL: select * from employee where age > 30 and age < 40.\n");
		s2 = tableScanRangeSelector(&head, employee_dictID, "age", "30", "40");
		printf("\nTinyDB> Your choise: ");
		fileID = s2;
	}
	else if (demonstrationCh == 5){
		// 投影
		printf("TinyDB> SQL: select rid, did from employee.\n");
		char* attribute_name = "rid|did";
		p = projection(&head, employee_dictID, attribute_name);
		fileID = p;
		printf("\nTinyDB> Your choise: ");
	}
	else if (demonstrationCh == 6){
		printf("TinyDB> SQL: select * from employee e, department d where e.did = d.did.\n");
		// 嵌套循环连接（在公共属性上的）
		j1 = nestedLoopJoin(&head, employee_dictID, department_dictID);
		// 基于排序的等值连接
		j2 = SortJoin(&head, employee_dictID, department_dictID);
		// 基于散列的等值连接
		j3 = HashJoin(&head, employee_dictID, department_dictID);
		fileID = j3;
		printf("\nTinyDB> Your choise: ");
	}
	else if (demonstrationCh == 7){
		// 嵌套循环连接（在公共属性上的）,三表连接
		printf("TinyDB> SQL: select * from employee e, department d, birthday b where e.did = d.did and d.idcard = b.idcard.\n");
		j4 = nestedLoopJoinByThree(&head, j1, department_dictID, birthday_dictID);
		fileID = j4;
		printf("\nTinyDB> Your choise: ");
	}
	else if (demonstrationCh == 8){
		// 不同数据类型的多条件等值连接（在公共属性上的）
		printf("TinyDB> SQL: select * from department d, birthday b where d.idcard = b.idcard and d.birthtime like '%19', 0-begin, 1-contains, 2-end.\n");
		j5 = nestedLoopJoinByConds(&head, birthday_dictID, department_dictID, "birthtime", "89", 1);
		fileID = j5;
		printf("\nTinyDB> Your choise: ");
	}
	else if (demonstrationCh == 9){
		// 不同数据类型的多条件非等值连接（在公共属性上的）
		printf("TinyDB> SQL: select * from department d, birthday b where d.idcard = b.idcard and d.birthtime like '%19', 0-begin, 1-contains, 2-end, 3-not contains.\n");
		j6 = nestedLoopJoinByConds(&head, birthday_dictID, department_dictID, "birthtime", "89", 3);
		fileID = j6;
		printf("\nTinyDB> Your choise: ");
	}
	else if (demonstrationCh == 10){
		// 测试用逻辑号去找某条记录
		char des[1000];
		for (int i = 0; i < 10; i++) {
			if (queryRecordByLogicID(&head, i * 15, des) < 0)
				continue;
			printf("LogicID %d's content: %s\n", i * 15, des);
		}
		printf("\nTinyDB> Your choise: ");
	}
	else if (demonstrationCh == 11){
		show_MapTableFile(&head, head.desc.fid_MapTable);
		printf("\nTinyDB> Your choise: ");
	}
	else if (demonstrationCh == 12){
		show_SysDesc(&head);
	}
	else if (demonstrationCh == 13){
		printf("TinyDB> Input fileID: \n");
		scanf("%d", &fileID);
		readFile(&head, fileID);
		printf("\nTinyDB> Your choise: ");
	}
	else if (demonstrationCh == 14){
		// 更改文件名
		char time[100];
		changeTime(time, ".mat");
		rename(filename, time);
		printf("\nTinyDB> Your choise: ");
	}
	else if (demonstrationCh == 15){
		printf("\nTinyDB> Current file info: 0 - ");
		printf("%d", max_file);
		printf("\nTinyDB> Input File No:");
		int fileID;
		scanf("%d", &fileID);
		// 导出csv
		writeFile(&head, fileID);
		printf("\nTinyDB> Your choise: ");
	}
	else if (demonstrationCh == 16){
		showMain();
		operation(head, filename);
	}
	else if (demonstrationCh == 0){
		printf("TintDB> Bye. \n");
	}
	// init tables()
}
int back(){
	welcome();
	// Create DB
	char filename[100];
	strcpy(filename, PATH);
	strcat(filename, "database.mat\0");
	struct dbSysHead head;
	// Init DB
	initDB(&head, filename);
	showInitInfo(head);
	printf("    Do you want to fully use TinyDB, if 1, not 0. \n");
	printf("    Your choose: \n");
	int x;
	scanf("%d", &x);
	switch (x)
	{
	case 1:
		// Show Operation
		showMain();
		operation(head, filename);
		break;
	case 0:
		printf("    See you next time. \n");
		break;
	default:
		break;
	}
	return 0;
}