#include "storage.h" 
#include "Test.h" 

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
	printf("�����ļ�%d�ɹ���\n", fileID); 
	//int mapfileID = StorageManager.fileOpt.createFile(&dbhead, MAP_FILE, 1);

	memToDisk(&dbhead);
	StorageManager.showDbInfo(&dbhead);

	//int fileID = 0;
	int num = 200;
	for (int i = 0; i < num; i++) {
		struct Student stu = { i, "abc", 30 + i, 5000 + i };
		char str[1000];
		sprintf(str, "%ld", stu.rid);
		strcat(str, stu.name);
		printf("str: %s\n", str);
		char tmp[100];
		sprintf(tmp, "%d", stu.age);
		strcat(str, tmp);
		sprintf(tmp, "%d", stu.weight);
		printf("tmp: %s\n", tmp);
		strcat(str, tmp);
		printf("str: %s\n", str);
		StorageManager.fileOpt.writeFile(&dbhead, fileID, strlen(str), str);
	}

	char des[1000];
	StorageManager.fileOpt.readFile(&dbhead, fileID, des);

	memToDisk(&dbhead);
	StorageManager.showDbInfo(&dbhead);
	fclose(dbhead.dataPath);
	free(dbhead.FreeSpace_bitMap);

	// �����ļ���
	time_t tt = time(NULL);//��䷵�ص�ֻ��һ��ʱ��cuo
	tm* t = localtime(&tt);
	printf("%d-%02d-%02d %02d:%02d:%02d\n",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec); 
	char time[50];
	sprintf(time, "%d", t->tm_hour);
	strcat(time, t->tm_min + "");
	strcat(time, t->tm_sec + "");
	strcat(time, ".mat");
	rename(fileName, time);

	return 0;
}