#include "storage.h" 
#include "Test.h" 
#include <sstream>

int main(int argc, char *argv[])
{
	//printf("hello");
	Storage StorageManager;
	DbMetaHead dbhead; 
	char fileName[15] = "tinydb.mat";
	StorageManager.initDB(&dbhead, fileName);
	StorageManager.showDbInfo(&dbhead);

	StorageManager.fileOpt.deleteFile(&dbhead, 0);
	StorageManager.pageOpt.recyAllPage(&dbhead);
	StorageManager.showDbInfo(&dbhead);

	int fileID = StorageManager.fileOpt.createFile(&dbhead, NORMAL_FILE, 1);
	printf("创建文件%d成功！\n", fileID); 
	//int mapfileID = StorageManager.fileOpt.createFile(&dbhead, MAP_FILE, 1);

	memToDisk(&dbhead);
	StorageManager.showDbInfo(&dbhead);

	//int fileID = 0;
	/*int num = 2000;
	for (int i = 0; i < num; i++) {
		struct Student stu = { i, "abc", 30 + i, 5000 + i };
		char str[1000];
		sprintf(str, "%ld", stu.rid);
		strcat(str, stu.name); 
		char tmp[100];
		sprintf(tmp, "%d", stu.age); 
		sprintf(tmp, "%d", stu.weight); 
		strcat(str, tmp);
		if (i % 10 == 0)
			printf("str: %s\n", str);
		StorageManager.fileOpt.writeFile(&dbhead, fileID, strlen(str), str);
	}*/
	char str[1000];
	ifstream fin("H:\\SelfLearning\\SAI\\Course\\tinydbms\\SPJAlgorithm\\supplier.tbl");
	vector<string> v;
	while (!fin.eof())
	{
		string line;
		getline(fin, line);
		strcpy(str, line.c_str());
		StorageManager.fileOpt.writeFile(&dbhead, fileID, strlen(str), str);
	}

	char des[1000];
	StorageManager.fileOpt.readFile(&dbhead, fileID, des);

	memToDisk(&dbhead);
	StorageManager.showDbInfo(&dbhead);
	fclose(dbhead.dataPath);
	free(dbhead.FreeSpace_bitMap);

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
	char time[50];
	sprintf(time, "%d", t->tm_year + 1900);
	char tmp[10];
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
	rename(fileName, time);

	return 0;
}