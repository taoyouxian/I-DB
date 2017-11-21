#include "dbHead.h"

//给定逻辑号，找到其在映射表中的所在页，返回该页页头
long queryLogicID(struct dbSysHead *head, long logicID, struct pageHead *PHead){

	long size_maptable = sizeof(struct dbMapTable);
	long num = (SIZE_PER_PAGE - SIZE_PAGEHEAD) / size_maptable;
	long page = logicID / num;
	long pos = logicID - num * page;

	int fid = head->desc.fid_MapTable;
	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件！\n", fid);
		exit(0);
	}
	long curPageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long filepagenum = head->desc.fileDesc[fid].filePageNum;

	//找到第page页的页头PHead
	int i = 0;
	int mapNo;
	do{
		//读页头
		//todo：应该把映射表的所有页都pin在缓冲区中，使其不会被替换
		mapNo = reqPage(head, curPageNo);
		memcpy(PHead, head->buff.data[mapNo], SIZE_PAGEHEAD);
		long nextpid = PHead->nextPageNo;
		if (nextpid == -1)   //到最后一页了
			break;
		else
			curPageNo = nextpid;

		i++;
	} while (i <= page && i < filepagenum);

	return pos;
}

int queryRecordByLogicID(struct dbSysHead *head, long logicID, char *des) {
	long size_maptable = sizeof(struct dbMapTable);
	long page = logicID / (8 * sizeof(long));
	long pos = logicID - 8 * sizeof(long)*page + 1; 
	if (getBit(*(head->FreeLogicID_bitMap + page), pos) == LOGICID_AVAI){
		printf("逻辑号%ld尚未分配！\n", logicID);
		return -1;
	} 
	//得到该记录对应的映射表表项所在的页头，以及该表项在该页中的位置
	struct pageHead PHead;
	pos = queryLogicID(head, logicID, &PHead);
	int mapNo = reqPage(head, PHead.pageNo);

	struct dbMapTable mt;
	memcpy(&mt, head->buff.data[mapNo] + SIZE_PAGEHEAD + size_maptable * pos, size_maptable);
	if (mt.isdeleted){
		printf("逻辑号为%ld的记录已被删除，无法读取！\n", logicID);
		return -1;
	}
	long pageNo = mt.pageNo;
	long recordID = mt.recordID;
	mapNo = reqPage(head, pageNo);

	struct offsetInPage cur, pre;
	memcpy(&cur, head->buff.data[mapNo] + SIZE_PAGEHEAD + recordID * SIZE_OFFSET, SIZE_OFFSET);

	if (recordID > 0){
		memcpy(&pre, head->buff.data[mapNo] + SIZE_PAGEHEAD + (recordID - 1) * SIZE_OFFSET, SIZE_OFFSET);
		memcpy(des, head->buff.data[mapNo] + SIZE_PER_PAGE - cur.offset, cur.offset - pre.offset);
	}
	else
		memcpy(des, head->buff.data[mapNo] + SIZE_PER_PAGE - cur.offset, cur.offset);

	des[recordID > 0 ? cur.offset - pre.offset : cur.offset] = '\0';
	return 0;
}
//移动记录，其对应的物理地址会有变化（页号+记录号）
//给定逻辑号，更新其在映射表中的表项
//void updateMapTableByLogicID(struct dbSysHead *head, long logicID, long pageNo, long recordID) {
//    long size_maptable = sizeof(struct dbMapTable);
//    long page = logicID / (8*sizeof(long));
//    long pos = logicID - 8*sizeof(long)*page + 1;
//    
//    struct pageHead PHead = queryLogicID(head, logicID);
//    int mapNo = reqPage(head, PHead.pageNo);
//    struct dbMapTable mt;
//    memcpy(&mt, head->buff.data[mapNo] + SIZE_PAGEHEAD + size_maptable * pos, size_maptable);
//    if(mt.isdeleted){
//        printf("逻辑号为%ld的记录已被删除，无法读取！\n",logicID);
//        return;
//    }
//    mt.pageNo = pageNo;
//    mt.recordID = recordID;
//    memcpy(head->buff.data[mapNo] + SIZE_PAGEHEAD + size_maptable * pos, &mt, size_maptable);
//}

// 给定逻辑号，更新该记录的内容
void updateRecordByLogicID(struct dbSysHead *head, long logicID, char* str) {

}
//索引部分
void queryRecordByKey(struct dbSysHead *head, int fid, int key, char *des){

}

void insertRecordByKey(struct dbSysHead *head, int fid, char *str) {

}

void deleteRecordByKey(struct dbSysHead *head, int fid, int key) {

}

//给定页内记录号，获得记录内容，返回逻辑号
long getNextRecord(struct dbSysHead *head, int mapNo, int recordNo, char *des){
	struct offsetInPage curoffset;
	int length = 0;
	memcpy(&curoffset, head->buff.data[mapNo] + SIZE_PAGEHEAD + recordNo * SIZE_OFFSET, SIZE_OFFSET);
	long logicID = curoffset.logicID;
	if (recordNo == 0)
		length = curoffset.offset;
	else {
		struct offsetInPage preoffset;
		memcpy(&preoffset, head->buff.data[mapNo] + SIZE_PAGEHEAD + (recordNo - 1) * SIZE_OFFSET, SIZE_OFFSET);
		length = curoffset.offset - preoffset.offset;
	}
	memcpy(des, head->buff.data[mapNo] + SIZE_PER_PAGE - curoffset.offset, length);
	des[length] = '\0';
	return logicID;
}

long allocateLogicID(struct dbSysHead *head){
	long logicID;
	if (head->desc.curRecordNum >= MAX_LOGICID_NUM) {
		printf("该数据库中可插入的记录数已达到上限%d，无法插入新纪录！\n", MAX_LOGICID_NUM);
		exit(0);
	}

	long page, pos;
	int count = 0;
	long i;
	long total = MAX_LOGICID_NUM;

	for (i = 0; i < total; i++) {
		page = i / (8 * sizeof(long));
		pos = i - 8 * sizeof(long) * page + 1;
		count = 0;
		if (getBit(*(head->FreeLogicID_bitMap + page), pos) == LOGICID_AVAI) {
			count = 1;
			break;
		}
	}
	if (count != 1) {
		return ALLO_FAIL;
	}
	else {
		logicID = i;
		page = i / (8 * sizeof(long));
		pos = i - 8 * sizeof(long) * page + 1;
		setBit(head->FreeLogicID_bitMap + page, pos, LOGICID_UNAVAI);
		head->desc.curRecordNum++;
	}
	return logicID;
}

void recyOneLogicID(struct dbSysHead *head, long logicID){
	long page = logicID / (8 * sizeof(long));
	long pos = logicID - 8 * sizeof(long)*page + 1;
	if (getBit(*(head->FreeLogicID_bitMap + page), pos) == LOGICID_UNAVAI){
		setBit(head->FreeLogicID_bitMap + page, pos, LOGICID_AVAI);
	}
}

// 遍历一次映射表文件的所有页，回收每个isDeleted=true的逻辑号
void recyAllFreeLogicID(struct dbSysHead *head){
	int fid = head->desc.fid_MapTable;
	if (fid < 0) {
		printf("数据库中不存在映射表文件！\n");
		return;
	}
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	long curPageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long size_mt = sizeof(struct dbMapTable);
	for (int i = 0; i < pageNum; i++) {
		int mapNo = reqPage(head, curPageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
		for (int j = 0; j < ph.curRecordNum; j++) {
			struct dbMapTable mt;
			memcpy(&mt, head->buff.data[mapNo] + SIZE_PAGEHEAD + j * size_mt, size_mt);
			if (mt.isdeleted == true){
				recyOneLogicID(head, mt.logicID);
			}
		}
		if (ph.nextPageNo < 0)
			break;
		else
			curPageNo = ph.nextPageNo;
	}

}