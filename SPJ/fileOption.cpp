#include "dbHead.h"

//ֱ�ӽ��д��̲���������buff
int createFile(struct dbSysHead *head, int type, long reqPageNum){
	if (head->desc.curFileNum >= MAX_FILE_NUM){
		printf("�����ļ�ʧ��.��ǰ���ݿ��Ѵ�����%d���ļ����ﵽ���ֵ��\n", MAX_FILE_NUM);
		exit(0);
	}
	if (head->desc.AvaiPage < reqPageNum){
		printf("�����ļ�ʧ��.��ǰ���ݿ����ҳ��Ϊ%ld���޷�������ļ������ҳ��%ld��\n", head->desc.AvaiPage, reqPageNum);
		exit(0);
	}
	//�ҵ�fileDesc�е�һ�����õ��±��
	int fid;
	for (fid = 0; fid < MAX_FILE_NUM; fid++) {
		if ((head->desc).fileDesc[fid].fileType < 0) {
			break;
		}
	}
	head->desc.fileDesc[fid].fileType = type;
	//head->desc.fileDesc[i].fileID = i;

	head->desc.curFileNum++;
	//Ϊ�ļ�����������ҳ��alloStaPageΪ��һҳҳ��
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
		printf("�����ļ�ʧ��.û���㹻�������ռ䡣\n");
		exit(0);
	}
	return fid;
}

//ֱ�Ӵ��̲���������buff
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

		//����ҳͷ
		rewind(head->fpdesc);
		fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * newPageID, SEEK_SET);
		fwrite(&ph, SIZE_PAGEHEAD, 1, head->fpdesc);

		//������һҳ��ҳͷ����Ϊ��ҳ�����·���ģ����п����ڻ������У���˼�Ҫ���»���������Ҫ���´��̣�
		int mapNo = reqPage(head, preph->pageNo);
		memcpy(head->buff.data[mapNo], preph, SIZE_PAGEHEAD);
		head->buff.map[mapNo].isEdited = true;

		rewind(head->fpdesc);
		fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * preph->pageNo, SEEK_SET);
		fwrite(preph, SIZE_PAGEHEAD, 1, head->fpdesc);

		return ph;
	}
	printf("��չ�ļ��ռ�ʧ��.\n");
	exit(0);
}

void insertIntoMapFile(struct dbSysHead *head, struct dbMapTable mt, long logicID){
	long size_maptable = sizeof(struct dbMapTable);
	long num = (SIZE_PER_PAGE - SIZE_PAGEHEAD) / size_maptable;
	long page = logicID / num;
	long pos = logicID - num * page;
	int fid = head->desc.fid_MapTable;
	if (fid < 0) {
		printf("���ݿ��в�����ӳ����ļ�.\n");
		return;
	}
	long curPageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long filepagenum = head->desc.fileDesc[fid].filePageNum;
	int mapNo = 0;
	struct pageHead PHead;
	//�ҵ���pageҳ��ҳͷPHead
	int i = 0;
	do{
		//��ҳͷ
		mapNo = reqPage(head, curPageNo);
		memcpy(&PHead, head->buff.data[mapNo], SIZE_PAGEHEAD);
		long nextpid = PHead.nextPageNo;
		if (nextpid == -1)   //�����һҳ��
			break;
		else
			curPageNo = nextpid;

		i++;
		head->buff.map[mapNo].isPin = true;        //��ӳ��������ҳ��pin�ڻ������У�ʹ�䲻�ᱻ�滻
	} while (i <= page && i < filepagenum);

	// ��������Ӧ������ҳ�в����һ��
	if ((page == filepagenum && pos == 0) ||
		// ��Ӧ�����һҳ��β������һ���µı������ҳ�ռ䲻����
		(page == filepagenum - 1 && pos == PHead.curRecordNum && PHead.freeSpace < size_maptable)) {

		struct pageHead ph = extendFile(head, fid, &PHead);
		ph.curRecordNum = 1;
		ph.freeSpace -= size_maptable;

		//����ҳͷ
		mapNo = reqPage(head, ph.pageNo);
		memcpy(head->buff.data[mapNo], &ph, SIZE_PAGEHEAD);
		//��ӳ������ҳ�в���һ������
		memcpy(head->buff.data[mapNo] + SIZE_PAGEHEAD, &mt, size_maptable);
		head->buff.map[mapNo].isEdited = true;
		head->buff.map[mapNo].isPin = true; //�·����ҳ��Ҳpin����������.
	}
	//��ҳĩβ���пռ䣬ֱ�����������Ӽ���
	else if (page == filepagenum - 1 && pos == PHead.curRecordNum && PHead.freeSpace >= size_maptable) {
		memcpy(head->buff.data[mapNo] + SIZE_PAGEHEAD + pos * size_maptable, &mt, size_maptable);

		PHead.curRecordNum++;
		PHead.freeSpace -= size_maptable;
		memcpy(head->buff.data[mapNo], &PHead, SIZE_PAGEHEAD);
		head->buff.map[mapNo].isEdited = true;
	}
	// ����֮ǰ�Ѵ��ڵ��߼���
	else {
		memcpy(head->buff.data[mapNo] + SIZE_PAGEHEAD + pos * size_maptable, &mt, size_maptable);
		head->buff.map[mapNo].isEdited = true;
	}

}
// ��ʱĬ��д��ļ�¼����һ��С��ҳ��С����������һ����¼��ҳ��
//question������Ǳ䳤��¼�Ļ�������Ĳ���strӦ����ʲô��ʽ��

void writeFile(struct dbSysHead *head, int dictID, char *str){
	long logicID = allocateLogicID(head);
	if (logicID == ALLO_FAIL){
		printf("�����߼���ʧ��.\n");
		exit(0);
	}
	//ӳ����ļ��е�һ������
	struct dbMapTable mt;
	mt.isdeleted = false;
	mt.logicID = logicID;

	int fid = head->data_dict[dictID].fileID;
	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ�.\n", fid);
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
		//��ҳͷ
		mapNo = reqPage(head, curPageNo);
		memcpy(&PHead, head->buff.data[mapNo], SIZE_PAGEHEAD);

		//��ҳ�Ŀ���ռ䲻����һ�����ݺ�һ��ƫ�����ˣ�����һҳ
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

			if (PHead.curRecordNum == 0) {   //��ҳ�������κμ�¼
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

			//����ҳͷ
			memcpy(head->buff.data[mapNo], &PHead, SIZE_PAGEHEAD);

			mt.pageNo = PHead.pageNo;
			mt.recordID = curOffset.recordID;

			isFinished = true;
			break;
		}
	}
	//���ļ���û���ĸ�ҳ���㹻�Ŀռ����д�¸ü�¼����չ�ļ����ٷ���һ����ҳ
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

		//������ҳ��ҳͷ
		mapNo = reqPage(head, ph.pageNo);
		memcpy(head->buff.data[mapNo], &ph, SIZE_PAGEHEAD);

		mt.pageNo = ph.pageNo;
		mt.recordID = curOffset.recordID;
	}
	//����ƫ��������¼ͷ���ͼ�¼
	memcpy(head->buff.data[mapNo] + curOffsetPos, &curOffset, SIZE_OFFSET);
	memcpy(head->buff.data[mapNo] + curRecordPos, str, length);
	head->buff.map[mapNo].isEdited = true;
	rl->recordNum++;

	//����ӳ����ļ�
	insertIntoMapFile(head, mt, logicID);

}

void readFile(struct dbSysHead *head, int dictID){
	int fid = head->data_dict[dictID].fileID;
	if (fid < 0) {
		printf("���ݿ��в������ļ���Ϊ%d���ļ�.\n", fid);
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
		/*printf("\n--------ҳͷ��Ϣ-------\n");
		printf("ҳ�ţ�%ld\n", ph.pageNo);
		printf("ҳ�Ŀ���ռ䣺%ld\n", ph.freeSpace);
		printf("��ҳ��ǰ�洢�ļ�¼������%d\n", ph.curRecordNum);
		printf("�߼���|��¼��|ƫ����|�Ƿ�ɾ��|��¼����\n");*/

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
		printf("���ݿ��в������ļ���Ϊ%d���ļ�.\n", fid);
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

		/*printf("\n--------ҳͷ��Ϣ-------\n");
		printf("ҳ�ţ�%ld\n", ph.pageNo);
		printf("ҳ�Ŀ���ռ䣺%ld\n", ph.freeSpace);
		printf("��ҳ��ǰ�洢�ļ�¼������%d\n", ph.curRecordNum);
		printf("�߼���|��¼��|ƫ����|�Ƿ�ɾ��|��¼����\n");*/

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