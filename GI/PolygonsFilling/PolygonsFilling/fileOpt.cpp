#include "MainWindow.h"
#include <fstream>
#include "storage.h"
#include "pageOpt.h"
#include "bufOpt.h"
#include "SPJ.h"

#include <QFileDialog>
#include <QMessageBox>

using namespace std;

FileOpt::FileOpt(void)
{
}


FileOpt::~FileOpt(void)
{
}

Storage storage;
PageOpt page;
BufOpt buf;
SPJ spjOpt;

//ֱ�ӽ��д��̲���������buff
int FileOpt::createFile(struct dbSysHead *head, int type, long reqPageNum, MainWindow* ui){
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
	long alloStaPage = page.allocatePage(head, reqPageNum);
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

int FileOpt::createFile(struct dbSysHead *head, int type, long reqPageNum){
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
	long alloStaPage = page.allocatePage(head, reqPageNum);
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
struct pageHead FileOpt::extendFile(struct dbSysHead *head, int fid, struct pageHead *preph){
	long newPageID = page.allocatePage(head, 1);
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
		int mapNo = buf.reqPage(head, preph->pageNo);
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

void FileOpt::insertIntoMapFile(struct dbSysHead *head, struct dbMapTable mt, long logicID){
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
		mapNo = buf.reqPage(head, curPageNo);
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
		mapNo = buf.reqPage(head, ph.pageNo);
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

void FileOpt::writeFile(struct dbSysHead *head, int dictID, char *str, MainWindow* ui){
	long logicID = spjOpt.allocateLogicID(head);
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
		printf("The file number %d does not exist in the database.\n", fid);
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
		mapNo = buf.reqPage(head, curPageNo);
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
		mapNo = buf.reqPage(head, ph.pageNo);
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

void FileOpt::readFile(struct dbSysHead *head, int dictID, char* attribute_name, MainWindow* ui){
	int fid = head->data_dict[dictID].fileID;
	if (fid < 0) {
		printf("The file number %d does not exist in the database.\n", fid);
		return;
	}
	int type = head->desc.fileDesc[fid].fileType;
	if (type == USER_FILE){
		storage.show_Relation(head, dictID, ui);
	}
	else if (type == TMP_TABLE){
		printf("\nThis a temporary tables.\n");
	}
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	struct pageHead ph;
	struct offsetInPage curOffset, preOffset;
	int mapNo = -1;

	/*QString fileName = SaveFileDialog();
	if (fileName.isNull()) {
		;
	}
	ofstream fout(fileName.toStdString());
	*/

	char fileName[100];
	changeTime(fileName, ".csv");
	ofstream fout(fileName);
	int rowID = 1;  

	vector<std::string> vv;
	//���ñ���б���  
	QStringList headerLabels;
	Relation rl = head->data_dict[dictID];
	if (strcmp("*", attribute_name) == 0){
		for (int i = 0; i < rl.attributeNum; i++){
			char *name = rl.atb[i].getName();
			headerLabels << name;
		}
		ui->tableWidget->setColumnCount(rl.attributeNum);
	}else if (strcmp("count(*)", attribute_name) == 0){ 
			headerLabels << attribute_name; 
			ui->tableWidget->setColumnCount(1);
	}
	else
	{ 
		vv = InfoSplit(attribute_name, "|");
		for (int i = 0; i < vv.size(); i++){ 
			headerLabels << vv[i].c_str();
		}
		ui->tableWidget->setColumnCount(vv.size());
	} 
	
	//ui->tableWidget->setRowCount(rowID);

	//headerLabels << "C1" << "C2" << "C3" << "Ck" << "..." << "Cn-1" << "Cn";
	//ui->tableWidget->setGeometry(QRect(200, 360, 760, 250));   // X, Y, W, H
	ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
	//���ñ���б���Ķ��뷽ʽ  
	ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

	//�����б���  
	QStringList rowLabels;
	ui->tableWidget->setVerticalHeaderLabels(rowLabels);

	//�Զ��������һ�еĿ��ʹ���ͱ����ұ߽����  
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true); 

	vector<std::string> v;
	for (int i = 0; i < pageNum; i++){
		mapNo = buf.reqPage(head, pageNo);
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
		if (ph.curRecordNum == 0){
			ui->tableWidget->setRowCount(rowID-1);
		}
		for (int j = 0; j < ph.curRecordNum; j++) {
			memcpy(&curOffset, head->buff.data[mapNo] + SIZE_PAGEHEAD + SIZE_OFFSET * j, SIZE_OFFSET); 
			fout << curOffset.logicID << curOffset.recordID << "\t" << curOffset.offset << "\t" << curOffset.isDeleted << endl;
			if (curOffset.isDeleted == 0){
				if (j == 0)
					readLength = curOffset.offset;
				else
					readLength = curOffset.offset - preOffset.offset;
				char *des = (char*)malloc(readLength);
				memset(des, 0, readLength);
				memcpy(des, head->buff.data[mapNo] + SIZE_PER_PAGE - curOffset.offset, readLength);
				des[readLength] = '\0';
				fout << des << endl;


				char str1[25];
				itoa(rowID, str1, 10);

				ui->tableWidget->setRowCount(rowID);
				rowID++;
				rowLabels << str1;
				// for ѭ�� 
				v = InfoSplit(des, "|");
				int row = rowID - 2;
				QTableWidgetItem *item0 = new QTableWidgetItem;

				for (int z = 0; z < v.size(); z++){
					item0->setText(v[z].c_str());
					ui->tableWidget->setItem(row, z, item0);
					item0 = new QTableWidgetItem;
					//headerLabels << v[z].c_str();
				} 
				//ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
			} 
			preOffset = curOffset;
		}
		if (ph.nextPageNo < 0)
			break;
		else
			pageNo = ph.nextPageNo;
	}
	fout.close();
	//QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromLocal8Bit("Save Success."));
}

void FileOpt::deleteRecordByCompKey(struct dbSysHead *head, int dictID, char* attribute_name, char* compType, char* value, MainWindow* ui){
	int fid = head->data_dict[dictID].fileID;
	if (fid < 0) {
		printf("The file number %d does not exist in the database.\n", fid);
		return;
	}
	int type = head->desc.fileDesc[fid].fileType;
	if (type == USER_FILE){
		storage.show_Relation(head, dictID, ui);
	}
	else if (type == TMP_TABLE){
		printf("\nThis a temporary tables.\n");
	}
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	struct pageHead ph;
	struct offsetInPage curOffset, preOffset;
	int mapNo = -1;
	   
	Relation rl = head->data_dict[dictID];
	int index = rl.getAttributeIndexByName(attribute_name);

	vector<std::string> v;
	for (int i = 0; i < pageNum; i++){
		mapNo = buf.reqPage(head, pageNo);
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);

		/*fout << "-------- Page Head Info -------" << endl;
		fout << "PageNo" << "\t" << ph.pageNo << endl;
		fout << "PageFreePalce" << "\t" << ph.freeSpace << endl;
		fout << "RecordNum Stored in CurPage" << "\t" << ph.curRecordNum << endl;
		fout << "Offset Table ----- Record Content" << endl;
		fout << "LogicID" << "\t" << "RecordNo" << "\t" << "Offset" << "\t" << "IsDelete" << "\t" << "Record Content" << endl;
*/
		memcpy(&preOffset, head->buff.data[mapNo] + SIZE_PAGEHEAD, SIZE_OFFSET);
		int readLength = 0;
		int recordNum = ph.curRecordNum;
		for (int j = 0; j < ph.curRecordNum; j++) {
			memcpy(&curOffset, head->buff.data[mapNo] + SIZE_PAGEHEAD + SIZE_OFFSET * j, SIZE_OFFSET); 
			//fout << curOffset.logicID << curOffset.recordID << "\t" << curOffset.offset << "\t" << curOffset.isDeleted << endl;
			if (curOffset.isDeleted == 0){
				if (j == 0)
					readLength = curOffset.offset;
				else
					readLength = curOffset.offset - preOffset.offset;
				char *des = (char*)malloc(readLength);
				memset(des, 0, readLength);
				memcpy(des, head->buff.data[mapNo] + SIZE_PER_PAGE - curOffset.offset, readLength);
				des[readLength] = '\0';
				int flag = 0;
				v = InfoSplit(des, "|");
				char* val = const_cast<char*>(v[index].c_str());
				if (strcmp(compType, "=") == 0){
					if (strcmp(val, value) == 0){
						flag = 1;
					}
				}
				else if (strcmp(compType, ">=") == 0){
					if (strcmp(val, value) >= 0){
						flag = 1;
					}
				}
				else if (strcmp(compType, "<=") == 0){
					if (strcmp(val, value) <= 0){
						flag = 1;
					}
				}
				else if (strcmp(compType, "!=") == 0){
					if (strcmp(val, value) != 0){
						flag = 1;
					}
				}
				else if (strcmp(compType, ">") == 0){
					if (strcmp(val, value) > 0){
						flag = 1;
					}
				}
				else if (strcmp(compType, "<") == 0){
					if (strcmp(val, value) < 0){
						flag = 1;
					}
				}
				if (flag == 1){
					curOffset.isDeleted = 1;
					memcpy(head->buff.data[mapNo] + SIZE_PAGEHEAD + SIZE_OFFSET * j, &curOffset, SIZE_OFFSET);
					recordNum--;
					flag = 0;
				}
			}
			//fout << des << endl; 

			preOffset = curOffset;
		}
		ph.curDelNum = ph.curRecordNum - recordNum;
		// Ӱ����ڱ���
		memcpy(head->buff.data[mapNo], &ph, SIZE_PAGEHEAD);
		if (ph.nextPageNo < 0)
			break;
		else
			pageNo = ph.nextPageNo;
	} 
	//QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromLocal8Bit("Save Success."));
}

void FileOpt::deleteRecord(struct dbSysHead *head, int dictID, MainWindow* ui){
	int fid = head->data_dict[dictID].fileID;
	if (fid < 0) {
		printf("The file number %d does not exist in the database.\n", fid);
		return;
	}
	int type = head->desc.fileDesc[fid].fileType;
	if (type == USER_FILE){
		storage.show_Relation(head, dictID, ui);
	}
	else if (type == TMP_TABLE){
		printf("\nThis a temporary tables.\n");
	}
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	struct pageHead ph;
	struct offsetInPage curOffset, preOffset;
	int mapNo = -1;
	     
	vector<std::string> v;
	for (int i = 0; i < pageNum; i++){
		mapNo = buf.reqPage(head, pageNo);
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD); 
		memcpy(&preOffset, head->buff.data[mapNo] + SIZE_PAGEHEAD, SIZE_OFFSET);
		int readLength = 0;
		int recordNum = ph.curRecordNum;
		for (int j = 0; j < ph.curRecordNum; j++) {
			memcpy(&curOffset, head->buff.data[mapNo] + SIZE_PAGEHEAD + SIZE_OFFSET * j, SIZE_OFFSET); 
			if (curOffset.isDeleted == 0){
				/*if (j == 0)
					readLength = curOffset.offset;
				else
					readLength = curOffset.offset - preOffset.offset;
				char *des = (char*)malloc(readLength);
				memset(des, 0, readLength);
				memcpy(des, head->buff.data[mapNo] + SIZE_PER_PAGE - curOffset.offset, readLength);
				des[readLength] = '\0';*/ 

					curOffset.isDeleted = 1;
					memcpy(head->buff.data[mapNo] + SIZE_PAGEHEAD + SIZE_OFFSET * j, &curOffset, SIZE_OFFSET);
					recordNum--; 
			} 

			preOffset = curOffset;
		}
		ph.curDelNum = ph.curRecordNum - recordNum;
		// Ӱ����ڱ���
		memcpy(head->buff.data[mapNo], &ph, SIZE_PAGEHEAD);
		if (ph.nextPageNo < 0)
			break;
		else
			pageNo = ph.nextPageNo;
	}  
}

void FileOpt::writeFile(struct dbSysHead *head, int dictID, MainWindow* ui){
	int fid = head->data_dict[dictID].fileID;
	if (fid < 0) {
		printf("The file number %d does not exist in the database.\n", fid);
		return;
	}
	int type = head->desc.fileDesc[fid].fileType;
	if (type == USER_FILE){
		storage.show_Relation(head, dictID, ui);
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
		mapNo = buf.reqPage(head, pageNo);
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

QString FileOpt::OpenFileDialog() {
	QString fileName;
	fileName = QFileDialog::getOpenFileName(nullptr, QObject::tr("Open Config"), "", QObject::tr("Table Files (*.tbl)"));

	if (!fileName.isNull())
	{
		return fileName;
	}
	else  {
		return NULL;
	}
}
QString FileOpt::SaveFileDialog() {
	QString fileName;
	fileName = QFileDialog::getSaveFileName(nullptr, QObject::tr("Save Config"), "", QObject::tr("Page Files (*.txt)"));

	if (!fileName.isNull())
	{
		return fileName;
	}
	else {
		return NULL;
	}
}

void FileOpt::ReadFiletoPoints()
{
	QString fileName = OpenFileDialog();
	if (fileName.isNull()) {
		;
	}
	else { 
		int x, y; 
		ifstream fin(fileName.toStdString());
		while (!fin.eof()) {
			fin >> x >> y;
			if (x + y > 0) {  
			}
			else {  
			}
		}  
		fin.close();
	}
}

void FileOpt::LoadData(std::vector<supplier*> &suppliers)
{
	QString fileName = OpenFileDialog();
	if (fileName.isNull()) {
		;
	}
	else {
		supplier *p = NULL;
		string x;
		vector<std::string> v;
		ifstream fin(fileName.toStdString());
		while (!fin.eof()) {
			getline(fin, x);
			//fin >> x; 
			v = InfoSplit(x, "|");
			//for (int i = 0; i < v.size(); i++) {} 
			p = new supplier();
			p->S_superkey = atoi(v[0].c_str());
			strcpy(p->S_name, v[1].c_str());
			strcpy(p->S_address, v[2].c_str());
			p->S_nationkey = atoi(v[3].c_str());
			strcpy(p->S_phone, v[4].c_str());
			p->S_acctbal = atof(v[5].c_str());
			strcpy(p->S_comment, v[6].c_str());
			suppliers.push_back(p);
		}
		fin.close();
	}
}
void FileOpt::SavePolyAsFile()
{
	QString fileName = SaveFileDialog();
	if (fileName.isNull()) {
		;
	}
	else {
		ofstream fout(fileName.toStdString()); 
		fout.close();
		QMessageBox::about(NULL, QString::fromLocal8Bit("�ļ�"), QString::fromLocal8Bit("����ɹ�"));
	}
}

vector<std::string> FileOpt::InfoSplit(const string& s, const string& c)
{
	vector<std::string> v;
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
	return v;
}