#include "dbHead.h"

void show_SysDesc(struct dbSysHead *head){
	printf("\n******************* 数据库的相关参数 ****************\n");
	printf("数据库总大小：%ld\n", head->desc.sizeOfFile);
	printf("每一页的大小：%ld\n", head->desc.sizePerPage);
	printf("数据库总页数：%ld\n", head->desc.TotalPage);
	printf("当前可用页数：%ld\n", head->desc.AvaiPage);
	printf("空闲空间位示图的起始地址：%ld\n", head->desc.addr_FreeSpace_BitMap);
	printf("空闲空间位示图的大小：%ld\n", head->desc.size_FreeSpace_BitMap);
	printf("空闲逻辑号位示图的起始地址：%ld\n", head->desc.addr_FreeLogicID_BitMap);
	printf("空闲逻辑号位示图的大小：%ld\n", head->desc.size_FreeLogicID_BitMap);
	printf("数据区的起始地址：%ld\n", head->desc.addr_data);
	printf("映射表文件的文件号：%d\n", head->desc.fid_MapTable);
	printf("数据字典文件的文件号：%d\n", head->desc.fid_DataDictionary);
	printf("当前数据库的文件总数：%d\n", head->desc.curFileNum);
	printf("当前数据库中的记录总数：%ld\n", head->desc.curRecordNum);
	printf("\n**************************************************\n");
}

void show_Relation(struct dbSysHead *head, int dictID) {
	Relation rl = head->data_dict[dictID];
	printf("表名：%s\n", rl.relationName);
	printf("属性个数：%d\n", rl.attributeNum);
	printf("记录长度：%d\n", rl.recordLength);
	printf("记录个数：%d\n", rl.recordNum);
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
	printf("文件类型:");
	if (type == MAP_FILE)
		printf("映射表文件\n");
	else if (type == USER_FILE){
		printf("用户表文件\n");
		show_Relation(head, dictID);
	}
	else
		printf("索引文件\n");
	printf("该文件一共占用：%ld页\n", pageNum);
	printf("该文件的起始页页号为：%ld\n", pageNo);
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
	printf("逻辑地址    物理地址\n");
	printf("逻辑号    页号|页内记录号\n");
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
	printf("**********   欢迎使用我们的数据库管理系统！    *********\n");
	printf("**********                               *********\n");
	printf("**************************************************\n");
	printf("**************************************************\n");
}

void exitdb(){
	printf("**************************************************\n");
	printf("**************************************************\n");
	printf("**********                               *********\n");
	printf("**********   谢谢使用我们的数据库管理系统！    *********\n");
	printf("**********                               *********\n");
	printf("**************************************************\n");
	printf("**************************************************\n");
}