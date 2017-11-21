#include "dbHead.h"

void show_SysDesc(struct dbSysHead *head){
	printf("\n******************* Database related parameters ****************\n");
	printf("Total database size: %ld\n", head->desc.sizeOfFile);
	printf("The size of each page: %ld\n", head->desc.sizePerPage);
	printf("The total number of database pages: %ld\n", head->desc.TotalPage);
	printf("Currently available pages: %ld\n", head->desc.AvaiPage);
	printf("The starting address of free space: %ld\n", head->desc.addr_FreeSpace_BitMap);      // 空闲空间位示图的起始地址
	printf("The size of bitmap size in free space: %ld\n", head->desc.size_FreeSpace_BitMap);          // 空闲空间位示图的大小
	printf("The start address of the diagram in idle logic bit: %ld\n", head->desc.addr_FreeLogicID_BitMap);  // 空闲逻辑号位示图的起始地址
	printf("The size of the bitmap in idle logic number: %ld\n", head->desc.size_FreeLogicID_BitMap);      // 空闲逻辑号位示图的大小
	printf("The starting address of the data area: %ld\n", head->desc.addr_data);
	printf("The file number of the mapping table file: %d\n", head->desc.fid_MapTable);
	printf("File number of the data dictionary file: %d\n", head->desc.fid_DataDictionary);
	printf("The total number of files in the current database: %d\n", head->desc.curFileNum);
	printf("The total number of records in the current database: %ld\n", head->desc.curRecordNum);
	printf("\n**************************************************\n");
}

void show_Relation(struct dbSysHead *head, int dictID) {
	Relation rl = head->data_dict[dictID];
	printf("Tablename: %s\n", rl.relationName);
	printf("Attributes number: %d\n", rl.attributeNum);
	printf("Record length: %d\n", rl.recordLength);
	printf("Record number: %d\n", rl.recordNum);
}

void show_FileDesc(struct dbSysHead *head, int dictID) {
	int fid = head->data_dict[dictID].fileID;
	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件！\n", fid);
		return;
	}
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	int type = head->desc.fileDesc[fid].fileType;
	printf("\n**************************************************\n");
	printf("File Type: ");
	if (type == MAP_FILE)
		printf("Mapping file\n");
	else if (type == USER_FILE){
		printf("User file\n");
		show_Relation(head, dictID);
	}
	else
		printf("Index file\n");
	printf("The file occupies: %ld page\n", pageNum);
	printf("The starting page number of the file is: %ld\n", pageNo);
	printf("\n**************************************************\n");

}

void show_MapTableFile(struct dbSysHead *head, int fid) {
	show_FileDesc(head, fid);
	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件！\n", fid);
		return;
	}
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	long curPageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long size_ph = sizeof(struct pageHead);
	long size_mt = sizeof(struct dbMapTable);
	printf("\n**************************************************\n");
	printf("LogicAddress\tPhysicalAddress\n");
	printf("LogicID\tPageNo|Page RecoredID\n");
	for (int i = 0; i < pageNum; i++) {
		int mapNo = reqPage(head, curPageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], size_ph);
		for (int j = 0; j < ph.curRecordNum; j++) {
			struct dbMapTable mt;
			memcpy(&mt, head->buff.data[mapNo] + size_ph + j * size_mt, size_mt);
			printf("%ld\t%ld\t%ld\n", mt.logicID, mt.pageNo, mt.recordID);
		}
		long nextpid = ph.nextPageNo;
		if (nextpid < 0)
			break;
		else
			curPageNo = nextpid;
	}
}

void welcome(){
	printf("**************************************************\n");
	printf("**************************************************\n");
	printf("**********                               *********\n");
	printf("**********   Welcome to use [TinyDB]     *********\n");
	printf("**********                               *********\n");
	printf("**************************************************\n");
	printf("**************************************************\n");
}

void exitdb(){
	printf("**************************************************\n");
	printf("**************************************************\n");
	printf("**********                               *********\n");
	printf("**********   Thanks for using [TinyDB]   *********\n");
	printf("**********                               *********\n");
	printf("**************************************************\n");
	printf("**************************************************\n");
}

void changeTime(char *target, char *type){
	// 更改文件名
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	/*printf("%d-%02d-%02d %02d:%02d:%02d\n",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);*/
	strcpy(target, PATH);
	strcat(target, "logs\\");
	char tmp[10];
	sprintf(tmp, "%d", t->tm_year + 1900);
	strcat(target, tmp);
	sprintf(tmp, "-%d", t->tm_mon + 1);
	strcat(target, tmp);
	sprintf(tmp, "-%d", t->tm_mday);
	strcat(target, tmp);
	sprintf(tmp, " %d", t->tm_min);
	strcat(target, tmp);
	sprintf(tmp, "_%d", t->tm_sec);
	strcat(target, tmp);
	sprintf(tmp, "_%d", t->tm_sec);
	strcat(target, tmp);
	strcat(target, type);
}