#include "storage.h"

#include <QtWidgets\QAction>
#include <QtWidgets\QMenu>
#include <QtWidgets\QScrollArea>
#include <QtWidgets\QMenuBar> 
#include <QtWidgets\QApplication>
#include <QFileDialog>
#include <QMessageBox> 
#include "MainWindow.h" 

Storage::Storage()
{
}


Storage::~Storage()
{
} 

void Storage::creatSysHead(char *filename, MainWindow* ui){
	struct dbSysHead sysHead; // 配置系统参数，并写入文件中

	// 存储文件中，数据区大小，默认为196MB
	sysHead.desc.sizeOfFile = SIZE_DATA_SPACE;
	// 每页的大小，4KB
	sysHead.desc.sizePerPage = SIZE_PER_PAGE;
	// 总页数
	sysHead.desc.TotalPage = (SIZE_DATA_SPACE) / (SIZE_PER_PAGE);
	// 当前可用的页数，初始化时=totalPage
	sysHead.desc.AvaiPage = sysHead.desc.TotalPage;

	// 空闲空间位示图的起始地址
	sysHead.desc.addr_FreeSpace_BitMap = ADDR_FREE_SPACE_BIT_MAP;
	// 大小，每页用1bit，一共需要"页数"个bit，即"页数/8"个字节
	sysHead.desc.size_FreeSpace_BitMap = sysHead.desc.TotalPage / 8;	//byte

	// 空闲逻辑号位示图的起始地址
	sysHead.desc.addr_FreeLogicID_BitMap = ADDR_FREE_LOGICID_BIT_MAP;
	// 其大小，暂时静态分配1024000个可用逻辑号
	sysHead.desc.size_FreeLogicID_BitMap = SIZE_FREE_LOGICID_BIT_MAP;

	sysHead.desc.addr_data = ADDR_DATA;     //数据区起始位置，前面留给两个bitMap
	sysHead.desc.fid_MapTable = -1;
	sysHead.desc.fid_DataDictionary = -1;

	sysHead.desc.curRecordNum = 0;                      //当前已有的记录总数
	sysHead.desc.curFileNum = 0;
	memset(sysHead.desc.fileDesc, -1, sizeof(struct FileDesc) * MAX_FILE_NUM);

	FILE *fp = fopen(filename, "wb");
	rewind(fp);
	fwrite(&(sysHead.desc), sizeof(struct SysDesc), 1, fp);// 把初始化的sysHead相关参数写到文件头部
	// 为空闲空间映射表分配空间，所有的初始化为-1，表示空闲
	sysHead.FreeSpace_bitMap = (unsigned long *)malloc(sysHead.desc.size_FreeSpace_BitMap);
	memset(sysHead.FreeSpace_bitMap, -1, sysHead.desc.size_FreeSpace_BitMap);

	sysHead.FreeLogicID_bitMap = (unsigned long *)malloc(sysHead.desc.size_FreeLogicID_BitMap);
	memset(sysHead.FreeLogicID_bitMap, -1, sysHead.desc.size_FreeLogicID_BitMap);

	// 再写入到文件中
	rewind(fp);
	fseek(fp, sysHead.desc.addr_FreeSpace_BitMap, SEEK_SET);
	fwrite(sysHead.FreeSpace_bitMap, 1, sysHead.desc.size_FreeSpace_BitMap, fp);

	fseek(fp, sysHead.desc.addr_FreeLogicID_BitMap, SEEK_SET);
	fwrite(sysHead.FreeLogicID_bitMap, 1, sysHead.desc.size_FreeLogicID_BitMap, fp);

	fclose(fp);
}

void Storage::init_MapData(struct dbSysHead *head, MainWindow* ui){
	if (head->desc.fid_MapTable < 0){
		int MapTable_fid = fileOpt.createFile(head, MAP_FILE, 1);
		head->desc.fid_MapTable = MapTable_fid;
		printf("Create mapping table %d successfully.\n\n", MapTable_fid);
	}

	if (head->desc.fid_DataDictionary < 0) {
		int dataDict_fid = fileOpt.createFile(head, DATA_DICTIONARY, 1);
		head->desc.fid_DataDictionary = dataDict_fid;
		printf("Create data dictionary file %d successfully.\n\n", dataDict_fid);
	}
}
void Storage::showTables(struct dbSysHead *head, MainWindow* ui){
	string s1;
	QString info = QString::fromLocal8Bit("【") + "System Tables Info" + QString::fromLocal8Bit("】");
	for (int i = 0; i < head->tableName.size(); i++){
		string tablename = head->tableName[i];
		int s = head->tableMap[tablename];
		string str = to_string(s);
		s1.append(str + "  " + tablename + "\n");
		for (int j = 0; j < head->data_dict[s].attributeNum; j++){
			string tab = head->data_dict[s].atb[j].getName();
			s1.append("\t" + tab + "\n");
		}
		s1.append("-------------------------------\n");
	}
	ui->sysTables->setText(info + "\n- - - - - - - - - - - - - - - -\n" + QString::fromStdString(s1));
}

int Storage::init_Table(struct dbSysHead *head, MainWindow* ui){
	// 创建表supplier
	int supplier_dictID = spjOpt.createTable_supplier(head);
	if (supplier_dictID < 0){
		printf("Create table supplier failed.\n");
		exit(0);
	}
	char str1[3000];
	int recordLen1 = head->data_dict[supplier_dictID].recordLength;
	ifstream fin("H:\\SelfLearning\\SAI\\Course\\tinydbms\\QueryAccess\\supplier.tbl");
	while (!fin.eof())
	{ 
		string line;
		getline(fin, line);
		int min = recordLen1 - line.length();
		string temp(min, ' ');
		line.append(temp);  
		strcpy(str1, line.c_str());
		fileOpt.writeFile(head, supplier_dictID, str1, ui);
	} 
	head->tableName.push_back("supplier");
	head->tableMap["supplier"] = supplier_dictID;

	// 创建表partsupp
	int partsupp_dictID = spjOpt.createTable_partsupp(head);
	if (partsupp_dictID < 0){
		printf("Create table partsupp failed.\n");
		exit(0);
	}
	char str[300];
	int recordLen = head->data_dict[partsupp_dictID].recordLength;
	ifstream fin2("H:\\SelfLearning\\SAI\\Course\\tinydbms\\QueryAccess\\partsupp.tbl");
	while (!fin2.eof())
	{
		string line;
		getline(fin2, line);
		int min = recordLen - line.length();
		string temp(min, ' ');
		line.append(temp);
		strcpy(str, line.c_str());
		fileOpt.writeFile(head, partsupp_dictID, str, ui);
	}
	head->tableName.push_back("partsupp");
	head->tableMap["partsupp"] = partsupp_dictID;


	// 创建表nation
	int nation_dictID = spjOpt.createTable_nation(head);
	if (nation_dictID < 0){
		printf("Create table nation failed.\n");
		exit(0);
	}
	char str3[300];
	int recLen = head->data_dict[nation_dictID].recordLength;
	ifstream fin3("H:\\SelfLearning\\SAI\\Course\\tinydbms\\QueryAccess\\nation.tbl");
	while (!fin3.eof())
	{
		string line;
		getline(fin3, line);
		int min = recLen - line.length();
		string temp(min, ' ');
		line.append(temp);
		strcpy(str3, line.c_str());
		fileOpt.writeFile(head, nation_dictID, str3, ui);
	}
	head->tableName.push_back("nation");
	head->tableMap["nation"] = nation_dictID;
	return supplier_dictID;
}

void Storage::init_database(struct dbSysHead *head, char *filename, MainWindow* ui){
	printf("\nInit database...\n");
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL){
		printf("database not exist, start to create database...\n");
		creatSysHead(filename, ui);
		fp = fopen(filename, "rb");
	}
	//初始化head->desc
	rewind(fp);
	fread(&(head->desc), sizeof(struct SysDesc), 1, fp);
	//分配空间，并初始化
	head->FreeSpace_bitMap = (unsigned long *)malloc(head->desc.size_FreeSpace_BitMap);
	rewind(fp);
	fseek(fp, head->desc.addr_FreeSpace_BitMap, SEEK_SET); // 找到文件中写bitMap的位置，让fp指向这里
	fread(head->FreeSpace_bitMap, 1, head->desc.size_FreeSpace_BitMap, fp);// 从文件中读取bitMap的内容

	head->FreeLogicID_bitMap = (unsigned long*)malloc(head->desc.size_FreeLogicID_BitMap);
	fseek(fp, head->desc.addr_FreeLogicID_BitMap, SEEK_SET);
	fread(head->FreeLogicID_bitMap, 1, head->desc.size_FreeLogicID_BitMap, fp);

	fclose(fp);
	head->fpdesc = fopen(filename, "rb+");

	// 初始化缓冲区
	// 将每个缓冲区块都初始化（存储的文件的页号为-1，访问时间为0，标记为未修改过）
	for (int i = 0; i < SIZE_BUFF; i++) {
		head->buff.map[i].pageNo = -1;
		head->buff.map[i].visitTime = 0;
		head->buff.map[i].isEdited = false;
		head->buff.map[i].isPin = false;
		memset(head->buff.data[i], 0, SIZE_PER_PAGE);
	}
	head->buff.curTimeStamp = 0;

	int count = 0;
	//从数据字典文件中读取关系模式，再初始化data_dict
	if (head->desc.fid_DataDictionary >= 0){
		count = readDataDictionaryFile(head);
	}

	for (int i = count; i < MAX_FILE_NUM; i++) {
		memset(&head->data_dict[i], 0, sizeof(Relation));
		head->data_dict[i].fileID = -1;
	}
	printf("Init database successfully.\n");
}
int Storage::readDataDictionaryFile(struct dbSysHead *head) {
	int fid = head->desc.fid_DataDictionary;
	if (fid < 0) {
		printf("Data dictionary file does not exist.");
		return 0;
	}
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	int count = 0;
	for (long i = 0; i < pageNum; i++) {
		int mapNo = bufOpt.reqPage(head, pageNo);
		struct pageHead ph;
		memcpy(&ph, head->buff.data[mapNo], SIZE_PAGEHEAD);
		for (int j = 0; j < ph.curRecordNum; j++) {
			memcpy(&head->data_dict[count], head->buff.data[mapNo] + SIZE_PAGEHEAD + j * sizeof(Relation), sizeof(Relation));
			count++;
		}
		if (ph.nextPageNo < 0)
			break;
		else
			pageNo = ph.nextPageNo;
	}
	printf("Read the exist schema in current database successfully.\n");
	return count;
}

void Storage::show_MapTableFile(struct dbSysHead *head, int fid, MainWindow* ui) {
	show_FileDesc(head, fid, ui);
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
	printf("LogicID\tPageNo\tPage RecoredID\n");
	for (int i = 0; i < pageNum; i++) {
		int mapNo = bufOpt.reqPage(head, curPageNo);
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


void Storage::show_SysDesc(struct dbSysHead *head, MainWindow* ui){
	ui->fileLog.append("******************* Database related parameters ****************\n");
	ui->fileLog.append("Total database size: ");
	ui->fileLog.append(to_string(head->desc.sizeOfFile));
	ui->fileLog.append("\n");
	ui->fileLog.append("The size of each page: ");
	ui->fileLog.append(to_string(head->desc.sizePerPage));
	ui->fileLog.append("\n");
	ui->fileLog.append("The total number of database pages: ");
	ui->fileLog.append(to_string(head->desc.TotalPage));
	ui->fileLog.append("\n");
	ui->fileLog.append("Currently available pages: ");
	ui->fileLog.append(to_string(head->desc.AvaiPage));
	ui->fileLog.append("\n");
	ui->fileLog.append("The starting address of free space: ");      // 空闲空间位示图的起始地址
	ui->fileLog.append(to_string(head->desc.addr_FreeSpace_BitMap));
	ui->fileLog.append("\n");
	ui->fileLog.append("The size of bitmap size in free space: ");          // 空闲空间位示图的大小
	ui->fileLog.append(to_string(head->desc.size_FreeSpace_BitMap));
	ui->fileLog.append("\n");
	ui->fileLog.append("The start address of the diagram in idle logic bit: ");  // 空闲逻辑号位示图的起始地址
	ui->fileLog.append(to_string(head->desc.addr_FreeLogicID_BitMap));
	ui->fileLog.append("\n");
	ui->fileLog.append("The size of the bitmap in idle logic number: ");      // 空闲逻辑号位示图的大小
	ui->fileLog.append(to_string(head->desc.size_FreeLogicID_BitMap));
	ui->fileLog.append("\n");
	ui->fileLog.append("The starting address of the data area: ");
	ui->fileLog.append(to_string(head->desc.addr_data));
	ui->fileLog.append("\n");
	ui->fileLog.append("The file number of the mapping table file: ");
	ui->fileLog.append(to_string(head->desc.fid_MapTable));
	ui->fileLog.append("\n");
	ui->fileLog.append("File number of the data dictionary file: ");
	ui->fileLog.append(to_string(head->desc.fid_DataDictionary));
	ui->fileLog.append("\n");
	ui->fileLog.append("The total number of files in the current database: ");
	ui->fileLog.append(to_string(head->desc.curFileNum));
	ui->fileLog.append("\n");
	ui->fileLog.append("The total number of records in the current database: ");
	ui->fileLog.append(to_string(head->desc.curRecordNum));
	ui->fileLog.append("\n**************************************************************\n");
	ui->sqlText->setText(QString::fromStdString(ui->fileLog));
}

void Storage::show_Relation(struct dbSysHead *head, int dictID, MainWindow* ui) {
	Relation rl = head->data_dict[dictID];
	ui->fileLog.append("Tablename: ");
	ui->fileLog.append(rl.relationName);
	ui->fileLog.append("\n");
	ui->fileLog.append("Attributes number: ");
	ui->fileLog.append(to_string(rl.attributeNum));
	ui->fileLog.append("\n");
	ui->fileLog.append("Record length: ");
	ui->fileLog.append(to_string(rl.recordLength));
	ui->fileLog.append("\n");
	ui->fileLog.append("Record number: ");
	ui->fileLog.append(to_string(rl.recordNum));
	ui->fileLog.append("\n");
}

void Storage::show_FileDesc(struct dbSysHead *head, int dictID, MainWindow* ui) {
	QString value = ui->loadText->toPlainText();

	int fid = head->data_dict[dictID].fileID;
	if (fid < 0) {
		printf("数据库中不存在文件号为%d的文件！\n", fid);
		return;
	}
	long pageNum = head->desc.fileDesc[fid].filePageNum;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	int type = head->desc.fileDesc[fid].fileType;
	ui->fileLog.append("**************************************************\n");
	ui->fileLog.append("File Type: ");
	if (type == MAP_FILE)
		ui->fileLog.append("Mapping file\n");
	else if (type == USER_FILE){
		ui->fileLog.append("User file\n");
		show_Relation(head, dictID, ui);
	}
	else
		ui->fileLog.append("Index file\n");
	ui->fileLog.append("The file occupies: ");
	ui->fileLog.append(to_string(pageNum));
	ui->fileLog.append(" page\n");
	ui->fileLog.append("The starting page number of the file is: ");
	ui->fileLog.append(to_string(pageNo));
	ui->fileLog.append("\n**************************************************\n");

	ui->sqlText->setText(QString::fromStdString(ui->fileLog));
}


void Storage::welcome(MainWindow* ui){
	ui->welcomeLog = ""; 
	ui->welcomeLog.append("**********************************************************************\n");
	ui->welcomeLog.append("**********************************************************************\n");
	ui->welcomeLog.append("********************                               *******************\n");
	ui->welcomeLog.append("********************     Welcome to use [I*DB]     *******************\n");
	ui->welcomeLog.append("********************                               *******************\n");
	ui->welcomeLog.append("**********************************************************************\n");
	ui->welcomeLog.append("**********************************************************************\n");
	ui->sqlText->setText(QString::fromStdString(ui->welcomeLog));
}

void Storage::exitdb(MainWindow* ui){
	ui->welcomeLog = "";
	ui->welcomeLog.append("**********************************************************************\n");
	ui->welcomeLog.append("**********************************************************************\n");
	ui->welcomeLog.append("********************                               *******************\n");
	ui->welcomeLog.append("********************     Thanks for using [I*DB]   *******************\n");
	ui->welcomeLog.append("********************                               *******************\n");
	ui->welcomeLog.append("**********************************************************************\n");
	ui->welcomeLog.append("**********************************************************************\n");
	ui->sqlText->setText(QString::fromStdString(ui->welcomeLog));
}


//更新，把内存中的所有数据写入到磁盘中
int Storage::sysUpdate(struct dbSysHead *head){
	//表模式信息，写回磁盘（原内容都覆盖）
	int fid = head->desc.fid_DataDictionary;
	long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
	struct pageHead ph;
	fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * pageNo, SEEK_SET);
	fread(&ph, SIZE_PAGEHEAD, 1, head->fpdesc);
	ph.curRecordNum = 0;
	ph.freeSpace = SIZE_PER_PAGE - SIZE_PAGEHEAD;

	for (int i = 0; i < MAX_FILE_NUM; i++) {
		int fid_ = head->data_dict[i].fileID;
		if (fid_ < 0 || head->desc.fileDesc[fid_].fileType < 0){
			continue;
		}
		//删除临时表文件，不写回
		if (head->desc.fileDesc[fid_].fileType == TMP_TABLE){
			deleteFile(head, fid_);
			continue;
		}
		if (ph.freeSpace < sizeof(Relation)){
			if (ph.nextPageNo < 0)
				ph = fileOpt.extendFile(head, head->desc.fid_DataDictionary, &ph);
			else {
				pageNo = ph.nextPageNo;
				fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * pageNo, SEEK_SET);
				fread(&ph, SIZE_PAGEHEAD, 1, head->fpdesc);
			}
		}
		//插入新记录（表模式）
		fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * ph.pageNo + SIZE_PAGEHEAD + sizeof(Relation) * ph.curRecordNum, SEEK_SET);
		fwrite(&head->data_dict[i], sizeof(Relation), 1, head->fpdesc);

		ph.curRecordNum++;
		ph.freeSpace -= sizeof(Relation);
	}
	//更新页头
	fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * ph.pageNo, SEEK_SET);
	fwrite(&ph, SIZE_PAGEHEAD, 1, head->fpdesc);


	//首部信息，写回
	rewind(head->fpdesc);
	fwrite(&(head->desc), sizeof(struct SysDesc), 1, head->fpdesc);

	//空闲空间位示图，写回
	rewind(head->fpdesc);
	fseek(head->fpdesc, head->desc.addr_FreeSpace_BitMap, SEEK_SET);
	fwrite(head->FreeSpace_bitMap, sizeof(char), head->desc.size_FreeSpace_BitMap, head->fpdesc);

	//回收所有删除标志位为1的逻辑号
	spjOpt.recyAllFreeLogicID(head);

	//逻辑号位示图，写回
	fseek(head->fpdesc, head->desc.addr_FreeLogicID_BitMap, SEEK_SET);
	fwrite(head->FreeLogicID_bitMap, sizeof(char), head->desc.size_FreeLogicID_BitMap, head->fpdesc);

	return 0;
}
void Storage::deleteFile(struct dbSysHead *head, int fid){
	if (fid >= 0) {
		long pageNum = head->desc.fileDesc[fid].filePageNum;
		long pageNo = head->desc.fileDesc[fid].fileFirstPageNo;
		long pageAddr = head->desc.addr_data + pageNo * SIZE_PER_PAGE;
		long nextPage = -1;

		for (long i = 0; i < pageNum; i++) {
			struct pageHead ph;
			rewind(head->fpdesc);
			fseek(head->fpdesc, pageAddr, SEEK_SET);
			fread(&ph, SIZE_PAGEHEAD, 1, head->fpdesc);

			nextPage = ph.nextPageNo;
			pageOpt.recyOnePage(head, ph.pageNo);

			if (nextPage < 0)
				break;
			else
				pageAddr = head->desc.addr_data + nextPage * SIZE_PER_PAGE;
		}

		head->desc.AvaiPage += pageNum;
		head->desc.curFileNum--;
		head->desc.fileDesc[fid].fileType = -1;
		//        head->desc.fileDesc[fx].fileID = -1;
		head->desc.fileDesc[fid].fileFirstPageNo = -1;
		head->desc.fileDesc[fid].filePageNum = -1;
	}
	else{
		printf("数据库中不存在文件号为%d的文件！\n", fid);
	}
}
void Storage::deleteDataDict(struct dbSysHead *head, int fid) {
	for (int i = 0; i < MAX_FILE_NUM; i++) {
		if (head->data_dict[i].fileID == fid){
			memset(&head->data_dict[i], 0, sizeof(Relation));
			head->data_dict[i].fileID = -1;
		}
	}
}
void Storage::close_database(struct dbSysHead* head, MainWindow* ui){
	//首先把缓冲区里的所有内容写回磁盘
	for (int i = 0; i < SIZE_BUFF; i++) {
		if (head->buff.map[i].isEdited == true) {
			rewind(head->fpdesc);
			fseek(head->fpdesc, head->desc.addr_data + SIZE_PER_PAGE * head->buff.map[i].pageNo, SEEK_SET);
			fwrite(head->buff.data[i], sizeof(char), SIZE_PER_PAGE, head->fpdesc);
			head->buff.map[i].isEdited = false;
		}
	}
	show_SysDesc(head, ui);
	sysUpdate(head);
	show_SysDesc(head, ui);
	fclose(head->fpdesc);
	free(head->FreeSpace_bitMap);
	free(head->FreeLogicID_bitMap);
	exitdb(ui);
}