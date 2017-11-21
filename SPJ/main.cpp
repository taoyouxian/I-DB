#include "dbHead.h"
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

	int employee_dictID = createTable_employee(&head);
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

	employee_dictID = 0;
	readFile(&head, employee_dictID);

	show_MapTableFile(&head, head.desc.fid_MapTable);

	// 测试用逻辑号去找某条记录
	char des[1000];
	for (int i = 0; i < 10; i++) {
		if (queryRecordByLogicID(&head, i * 15, des) < 0)
			continue;
		printf("LogicID%d's content: %s\n", i * 15, des);
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

	close_database(&head);

	// 更改文件名
	char time[100];
	changeTime(time, ".mat");
	rename(filename, time);

	system("pause");
	return 0;
}
