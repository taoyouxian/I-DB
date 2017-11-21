#include "dbHead.h"

int main() {
	welcome();
	char filename[100];
	strcpy(filename, PATH);
	strcat(filename, "database.mat\0");
	//remove("database.txt");

	struct dbSysHead head;
	init_database(&head, filename);

	show_SysDesc(&head);

	if (head.desc.fid_MapTable < 0){
		int MapTable_fid = createFile(&head, MAP_FILE, 1);
		head.desc.fid_MapTable = MapTable_fid;
		printf("创建映射表文件%d成功！\n\n", MapTable_fid);
	}

	if (head.desc.fid_DataDictionary < 0) {
		int dataDict_fid = createFile(&head, DATA_DICTIONARY, 1);;
		head.desc.fid_DataDictionary = dataDict_fid;
		printf("创建数据字典文件%d成功！\n\n", dataDict_fid);
	}

	int employee_dictID = createTable_employee(&head);
	if (employee_dictID < 0){
		printf("创建表employee失败\n");
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
		//传入的字符串形如”1|5|30|abc“
		writeFile(&head, employee_dictID, str /*, rh*/);

	}

	show_FileDesc(&head, employee_dictID);

	employee_dictID = 0;
	readFile(&head, employee_dictID);

	show_MapTableFile(&head, head.desc.fid_MapTable);

	//测试用逻辑号去找某条记录
	char des[1000];
	for (int i = 0; i < 10; i++) {
		if (queryRecordByLogicID(&head, i * 15, des) < 0)
			continue;
		printf("逻辑号为%d的记录内容为：%s\n", i * 15, des);
	}

	//等值选择
	//todo：
	//应该是输入select语句，解析出table_name，再去data_dict里面找到相应的relation
	//再从输入中（where后面）得到属性名以及value
	int s1 = tableScanEqualSelector(&head, employee_dictID, "did", "6");
	readFile(&head, s1);

	//范围选择
	int s2 = tableScanRangeSelector(&head, employee_dictID, "age", "30", "40");
	readFile(&head, s2);

	//投影
	//todo:
	//应该是输入“select 属性”语句，得到用户指定的属性名，再传入函数中
	char* attribute_name = "rid|did";
	int p = projection(&head, employee_dictID, attribute_name);
	readFile(&head, p);

	//创建表department
	int department_dictID = createTable_department(&head);
	if (department_dictID < 0){
		printf("创建表departmet失败\n");
		exit(0);
	}
	for (int i = 0; i < 10; i++) {
		int num = rand() % 20 + 5;
		struct department d = { i, i * 2, num, "defg" };
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
		//传入的字符串形如”1|5|30|defg“
		writeFile(&head, department_dictID, str);
	}
	show_FileDesc(&head, department_dictID);
	readFile(&head, department_dictID);

	//嵌套循环连接（在公共属性上的）
	int j1 = nestedLoopJoin(&head, employee_dictID, department_dictID);
	readFile(&head, j1);

	//基于排序的等值连接
	int j2 = SortJoin(&head, employee_dictID, department_dictID);
	readFile(&head, j2);

	//基于散列的等值连接
	int j3 = HashJoin(&head, employee_dictID, department_dictID);
	readFile(&head, j3);

	close_database(&head);


	// 更改文件名
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	printf("%d-%02d-%02d %02d:%02d:%02d\n",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);
	char time[100];
	strcpy(time, PATH);
	strcat(time, "logs//");
	char tmp[10];
	sprintf(tmp, "%d", t->tm_year + 1900);
	strcat(time, tmp);
	sprintf(tmp, "-%d", t->tm_mon + 1);
	strcat(time, tmp);
	sprintf(tmp, "-%d", t->tm_mday);
	strcat(time, tmp);
	sprintf(tmp, " %d", t->tm_min);
	strcat(time, tmp);
	sprintf(tmp, "_%d", t->tm_sec);
	strcat(time, tmp);
	sprintf(tmp, "_%d", t->tm_sec);
	strcat(time, tmp);
	strcat(time, ".mat");
	rename(filename, time);
	return 0;
}