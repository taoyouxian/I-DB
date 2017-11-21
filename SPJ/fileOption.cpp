#include "dbHead.h"

//直接进行磁盘操作，不用buff
int createFile(struct dbSysHead *head, int type, long reqPageNum){
	if (head->desc.curFileNum >= MAX_FILE_NUM){
		printf("创建文件失败.当前数据库已创建了%d个文件，达到最大值。\n", MAX_FILE_NUM);
		exit(0);
	}
	if (head->desc.AvaiPage < reqPageNum){
		printf("创建文件失败.当前数据库可用页数为%ld，无法满足该文件请求的页数%ld。\n", head->desc.AvaiPage, reqPageNum);
		exit(0);
	}
	//找到fileDesc中第一个可用的下标号
	int fid;
	for (fid = 0; fid < MAX_FILE_NUM; fid++) {
		if ((head->desc).fileDesc[fid].fileType < 0) {
			break;
		}
	}
	head->desc.fileDesc[fid].fileType = type;
	//head->desc.fileDesc[i].fileID = i;

	head->desc.curFileNum++;
	//为文件分配连续的页，alloStaPage为第一页页号
	long alloStaPage = allocatePage(head, reqPageNum);
	if (alloStaPage >= 0){
		for (long i = 0; i < reqPageNum; i++) {
			struct pageHead ph;
			ph.pageNo = alloStaPage + i;
			ph.curRecordNum = 0;
			if (i == 0) {
				ph.prePageNo = -1;
				if (reqPageNum == 1)
					ph.nextPageNo = -1;
				else
					ph.nextPageNo = ph.pageNo + 1;
			}
			else {
				ph.prePageNo = ph.pageNo - 1;
				if (i == reqPageNum - 1)
					ph.nextPageNo = -1;
				else
					ph.nextPageNo = ph.pageNo + 1;
			}
			ph.freeSpace = SIZE_PER_PAGE - SIZE_PAGEHEAD;

			rewind(head->fpdesc);
			fseek(head->fpdesc, head->desc.addr_data + ph.pageNo * SIZE_PER_PAGE, SEEK_SET);
			fwrite(&ph, sizeof(ph), 1, head->fpdesc);
		}

		head->desc.fileDesc[fid].fileFirstPageNo = alloStaPage;
		head->desc.fileDesc[fid].filePageNum = reqPageNum;
	}
	else {
		printf("创建文件失败.没有足够的连续空间。\n");
		exit(0);
	}
	return fid;
}

//直接磁盘操作，不用buff
struct pageHead extendFile(struct dbSysHead *head, int fid, struct pageHead *preph){
	long newPageID = allocatePage(head, 1);
	if (newPageID >= 0) {
		head->desc.fileDesc[fid].filePageNum++;

		struct pageHead ph;
		ph.pageNo = newPageID;
		ph.prePageNo = preph->pageNo;
		ph.nextPageNo = -1;
		preph->nextPageNo = newPageID;
		ph.curRecordNum = 0;
		ph.freeSpace = SIZE_PER_PAGE - SIZE_PAGEHEAD;

		//更新页头
		rewind(head->fpdesc);
		fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * newPageID, SEEK_SET);
		fwrite(&ph, SIZE_PAGEHEAD, 1, head->fpdesc);

		//更新上一页的页头（因为该页不是新分配的，很有可能在缓冲区中，因此既要更新缓冲区，又要更新磁盘）
		int mapNo = reqPage(head, preph->pageNo);
		memcpy(head->buff.data[mapNo], preph, SIZE_PAGEHEAD);
		head->buff.map[mapNo].isEdited = true;

		rewind(head->fpdesc);
		fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * preph->pageNo, SEEK_SET);
		fwrite(preph, SIZE_PAGEHEAD, 1, head->fpdesc);

		return ph;
	}
	printf("扩展文件空间失败.\n");
	exit(0);
}

void insertIntoMapFile(struct dbSysHead *head, struct dbMapTable mt, long logicID){
	long size_maptable = sizeof(struct dbMapTable);
	long num = (SIZE_PER_PAGE - SIZE_PAGEHEAD) / size_maptable;
	long page = logicID / num;
	long pos = logicID - num * page;
	int fid = head->desc.fid_MapTable;
	if (fid < 0) {
		printf("数据库中不存在映射表文件.\n");
		return;
	}
	long curPageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long filepagenum = head->desc.fileDesc[fid].filePageNum;
	int mapNo = 0;
	struct pageHead PHead;
	//找到第page页的页头PHead
	int i = 0;
	do{
		//读页头
		mapNo = reqPage(head, curPageNo);
		memcpy(&PHead, head->buff.data[mapNo], SIZE_PAGEHEAD);
		long nextpid = PHead.nextPageNo;
		if (nextpid == -1)   //到最后一页了
			break;
		else
			curPageNo = nextpid;

		i++;
		head->buff.map[mapNo].isPin = true;        //把映射表的所有页都pin在缓冲区中，使其不会被替换
	} while (i <= page && i < filepagenum);

	// 整除，即应该向新页中插入第一条
	if ((page == filepagenum && pos == 0) ||
		// 或应向最后一页的尾部插入一条新的表项，但该页空间不足了
		(page == filepagenum - 1 && pos == PHead.curRecordNum && PHead.freeSpace < size_maptable)) {

		struct pageHead ph = extendFile(head, fid, &PHead);
		ph.curRecordNum = 1;
		ph.freeSpace -= size_maptable;

		//更新页头
		mapNo = reqPage(head, ph.pageNo);
		memcpy(head->buff.data[mapNo], &ph, SIZE_PAGEHEAD);
		//向映射表的新页中插入一条表项
		memcpy(head->buff.data[mapNo] + SIZE_PAGEHEAD, &mt, size_maptable);
		head->buff.map[mapNo].isEdited = true;
		head->buff.map[mapNo].isPin = true; //新分配的页，也pin到缓冲区中.
	}
	//本页末尾还有空间，直接在最后面添加即可
	else if (page == filepagenum - 1 && pos == PHead.curRecordNum && PHead.freeSpace >= size_maptable) {
		memcpy(head->buff.data[mapNo] + SIZE_PAGEHEAD + pos * size_maptable, &mt, size_maptable);

		PHead.curRecordNum++;
		PHead.freeSpace -= size_maptable;
		memcpy(head->buff.data[mapNo], &PHead, SIZE_PAGEHEAD);
		head->buff.map[mapNo].isEdited = true;
	}
	// 更新之前已存在的逻辑号
	else {
		memcpy(head->buff.data[mapNo] + SIZE_PAGEHEAD + pos * size_maptable, &mt, size_maptable);
		head->buff.map[mapNo].isEdited = true;
	}

}
// 暂时默认写入的记录长度一定小于页大小（即不允许一条记录跨页）
//question：如果是变长记录的话，传入的参数str应该是什么形式？

void writeFile(struct dbSysHead *head, int dictID, char *str){
	long logicID = allocateLogicID(head);
	if (logicID == ALLO_FAIL){
		printf("分配逻辑号失败.\n");
		exit(0);
	}
	//映射表文件中的一条表项
	struct dbMapTable mt;
	mt.isdeleted = false;
	mt.logicID = logicID;

	int fid = head->data_dict[dictID].fileID;
	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件.\n", fid);
		return;
	}
	long PageNum = head->desc.fileDesc[fid].filePageNum;
	long curPageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	Relation *rl = &head->data_dict[dictID];
	int length = strlen(str);

	struct pageHead PHead;
	int mapNo = 0;
	long curRecordPos = 0, curOffsetPos = 0;
	struct offsetInPage curOffset, preOffset;

	bool isFinished = false;
	for (int i = 0; i < PageNum; i++) {
		//读页头
		mapNo = reqPage(head, curPageNo);
		memcpy(&PHead, head->buff.data[mapNo], SIZE_PAGEHEAD);

		//本页的空余空间不够加一条数据和一个偏移量了，找下一页
		if (PHead.freeSpace <= SIZE_OFFSET + length){
			long nextpid = PHead.nextPageNo;
			if (nextpid == -1)
				break;
			else {
				curPageNo = nextpid;
				continue;
			}
		}
		else {
			curOffset.logicID = logicID;
			curOffset.recordID = PHead.curRecordNum;
			curOffset.isDeleted = false;

			if (PHead.curRecordNum == 0) {   //该页中暂无任何记录
				curOffset.offset = length;
				curRecordPos = SIZE_PER_PAGE - length;
				curOffsetPos = SIZE_PAGEHEAD;
			}
			else {
				memcpy(&preOffset, head->buff.data[mapNo] + SIZE_PAGEHEAD + SIZE_OFFSET * (PHead.curRecordNum - 1), SIZE_OFFSET);
				curOffset.offset = preOffset.offset + length;
				curRecordPos = SIZE_PER_PAGE - preOffset.offset - length;
				curOffsetPos = SIZE_PAGEHEAD + SIZE_OFFSET * curOffset.recordID;
			}
			PHead.curRecordNum++;
			PHead.freeSpace = PHead.freeSpace - length - SIZE_OFFSET;

			//更新页头
			memcpy(head->buff.data[mapNo], &PHead, SIZE_PAGEHEAD);

			mt.pageNo = PHead.pageNo;
			mt.recordID = curOffset.recordID;

			isFinished = true;
			break;
		}
	}
	//该文件中没有哪个页有足够的空间可以写下该记录，扩展文件，再分配一个新页
	if (isFinished == false) {
		struct pageHead ph = extendFile(head, fid, &PHead);

		curOffset.logicID = logicID;
		curOffset.recordID = 0;
		curOffset.offset = length;
		curOffset.isDeleted = false;

		curRecordPos = SIZE_PER_PAGE - length;
		curOffsetPos = SIZE_PAGEHEAD;

		ph.curRecordNum = 1;
		ph.freeSpace = ph.freeSpace - SIZE_OFFSET - length;

		//更新新页的页头
		mapNo = reqPage(head, ph.pageNo);
		memcpy(head->buff.data[mapNo], &ph, SIZE_PAGEHEAD);

		mt.pageNo = ph.pageNo;
		mt.recordID = curOffset.recordID;
	}
	//插入偏移量、记录头部和记录
	memcpy(head->buff.data[mapNo] + curOffsetPos, &curOffset, SIZE_OFFSET);
	memcpy(head->buff.data[mapNo] + curRecordPos, str, length);
	head->buff.map[mapNo].isEdited = true;
	rl->recordNum++;

	//更新映射表文件
	insertIntoMapFile(head, mt, logicID);

}

void readFile(struct dbSysHead *head, int dictID){
	int fid = head->data_dict[dictID].fileID;
	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件.\n", fid);
		return;
	}
	int type = head->desc.fileDesc[fid].fileType;
	if (type == USER_FILE){
		show_Relation(head, dictID);
	}
	else if (type == TMP_TABLE){
		printf("\nThis a temporary tables.\n");
	}
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	struct pageHead ph;
	struct offsetInPage curOffset, preOffset;
	int mapNo = -1;

	for (int i = 0; i < pageNum; i++){
		mapNo = reqPage(head, pageNo);
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);

		cout << "-------- Page Head Info -------" << endl;
		cout << "PageNo" << "\t" << ph.pageNo << endl;
		cout << "PageFreePalce" << "\t" << ph.freeSpace << endl;
		cout << "RecordNum Stored in CurPage" << "\t" << ph.curRecordNum << endl;
		cout << "Offset Table ----- Record Content" << endl;
		cout << "LogicID" << "\t" << "RecordNo" << "\t" << "Offset" << "\t" << "IsDelete" << "\t" << "Record Content" << endl;
		/*printf("\n--------页头信息-------\n");
		printf("页号：%ld\n", ph.pageNo);
		printf("页的空余空间：%ld\n", ph.freeSpace);
		printf("该页当前存储的记录个数：%d\n", ph.curRecordNum);
		printf("逻辑号|记录号|偏移量|是否删除|记录内容\n");*/

		memcpy(&preOffset, head->buff.data[mapNo] + SIZE_PAGEHEAD, SIZE_OFFSET);
		int readLength = 0;

		for (int j = 0; j < ph.curRecordNum; j++) {
			memcpy(&curOffset, head->buff.data[mapNo] + SIZE_PAGEHEAD + SIZE_OFFSET * j, SIZE_OFFSET);
			printf("%ld\t%d\t%d\t%d\t", curOffset.logicID, curOffset.recordID, curOffset.offset, curOffset.isDeleted);
			if (j == 0)
				readLength = curOffset.offset;
			else
				readLength = curOffset.offset - preOffset.offset;
			char *des = (char*)malloc(readLength);
			memset(des, 0, readLength);
			memcpy(des, head->buff.data[mapNo] + SIZE_PER_PAGE - curOffset.offset, readLength);
			des[readLength] = '\0';
			printf("%s\n", des);
			preOffset = curOffset;
		}
		if (ph.nextPageNo < 0)
			break;
		else
			pageNo = ph.nextPageNo;
	}
}

void writeFile(struct dbSysHead *head, int dictID){
	int fid = head->data_dict[dictID].fileID;
	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件.\n", fid);
		return;
	}
	int type = head->desc.fileDesc[fid].fileType;
	if (type == USER_FILE){
		show_Relation(head, dictID);
	}
	else if (type == TMP_TABLE){
		printf("\nThis a temporary table outputed. \n");
	}
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	struct pageHead ph;
	struct offsetInPage curOffset, preOffset;
	int mapNo = -1;

	char fileName[100];
	changeTime(fileName, ".csv");
	printf("CSV Path: %s. \n", fileName);
	ofstream fout(fileName);
	for (int i = 0; i < pageNum; i++){
		mapNo = reqPage(head, pageNo);
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);

		fout << "-------- Page Head Info -------" << endl;
		fout << "PageNo" << "\t" << ph.pageNo << endl;
		fout << "PageFreePalce" << "\t" << ph.freeSpace << endl;
		fout << "RecordNum Stored in CurPage" << "\t" << ph.curRecordNum << endl;
		fout << "Offset Table ----- Record Content" << endl;
		fout << "LogicID" << "\t" << "RecordNo" << "\t" << "Offset" << "\t" << "IsDelete" << "\t" << "Record Content" << endl;

		/*printf("\n--------页头信息-------\n");
		printf("页号：%ld\n", ph.pageNo);
		printf("页的空余空间：%ld\n", ph.freeSpace);
		printf("该页当前存储的记录个数：%d\n", ph.curRecordNum);
		printf("逻辑号|记录号|偏移量|是否删除|记录内容\n");*/

		memcpy(&preOffset, head->buff.data[mapNo] + SIZE_PAGEHEAD, SIZE_OFFSET);
		int readLength = 0;

		for (int j = 0; j < ph.curRecordNum; j++) {
			memcpy(&curOffset, head->buff.data[mapNo] + SIZE_PAGEHEAD + SIZE_OFFSET * j, SIZE_OFFSET);
			//printf("%ld\t%d\t%d\t%d\t", curOffset.logicID, curOffset.recordID, curOffset.offset, curOffset.isDeleted);
			fout << curOffset.logicID << "\t" << curOffset.recordID << "\t" << curOffset.offset << "\t" << curOffset.isDeleted << "\t";

			if (j == 0)
				readLength = curOffset.offset;
			else
				readLength = curOffset.offset - preOffset.offset;
			char *des = (char*)malloc(readLength);
			memset(des, 0, readLength);
			memcpy(des, head->buff.data[mapNo] + SIZE_PER_PAGE - curOffset.offset, readLength);
			des[readLength] = '\0';
			//printf("%s\n", des);
			fout << des << endl;
			preOffset = curOffset;
		}
		if (ph.nextPageNo < 0)
			break;
		else
			pageNo = ph.nextPageNo;
	}
	fout << "-------- Page End -------" << endl;
}