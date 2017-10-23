#include "storage.h" 

//更新，把内存中的所有数据写入到磁盘中
int sysUpdate(struct dbSysHead *head){
	rewind(head->fpdesc);
	fwrite(&(head->desc), sizeof(struct SysDesc), 1, head->fpdesc);

	rewind(head->fpdesc);
	fseek(head->fpdesc, head->desc.bitMapAddr, SEEK_SET);
	fwrite(head->FreeSpace_bitMap, sizeof(char), head->desc.sizeBitMap, head->fpdesc);

	for (int i = 0; i<SIZE_BUFF; i++) {
		if (head->buff.map[i].isEdited == true) {
			rewind(head->fpdesc);
			fseek(head->fpdesc, head->desc.dataAddr + SIZE_PER_PAGE * head->buff.map[i].pageNo, SEEK_SET);
			fwrite(head->buff.data[i], sizeof(char), SIZE_PER_PAGE, head->fpdesc);
			head->buff.map[i].isEdited = false;
		}
	}
	return 0;
}

struct employee{
	long rid;
	char name[100];
	int age;
	int wage;
};

int main(int argc, char *argv[])
{
	//printf("hello");
	Storage StorageManager;
	dbSysHead dbhead; 
	char filename[100] = "demo.mat";
	StorageManager.init_database(&dbhead, filename);
	StorageManager.show_SysDesc(&dbhead);

	StorageManager.fileOpt.deleteFile(&dbhead, 0);
	StorageManager.pageOpt.recyAllPage(&dbhead);
	StorageManager.show_SysDesc(&dbhead);

	int fid = StorageManager.fileOpt.createFile(&dbhead, NORMAL_FILE, 1);
	printf("创建文件%d成功！\n", fid);

	int mapfid = StorageManager.fileOpt.createFile(&dbhead, MAP_FILE, 1);
	sysUpdate(&dbhead);
	StorageManager.show_SysDesc(&dbhead);

	//int fid = 0;
	for (int i = 0; i < 200000; i++) {
		struct employee emp1 = { i, "abc", 30 + i, 5000 + i };
		char str[1000];
		sprintf(str, "%ld", emp1.rid);
		strcat(str, emp1.name);
		char tmp[100];
		sprintf(tmp, "%d", emp1.age);
		strcat(str, tmp);
		sprintf(tmp, "%d", emp1.wage);
		strcat(str, tmp);
		StorageManager.fileOpt.writeFile(&dbhead, fid, strlen(str), str);
	}

	char des[1000];
	StorageManager.fileOpt.readFile(&dbhead, fid, des);

	sysUpdate(&dbhead);
	StorageManager.show_SysDesc(&dbhead);
	fclose(dbhead.fpdesc);
	free(dbhead.FreeSpace_bitMap);

	return 0;
}

