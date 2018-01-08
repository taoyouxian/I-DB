#pragma once

#ifndef DBHEAD_H_INCLUDED
#define DBHEAD_H_INCLUDED

#include "ui_glwidget.h"
#include "common.h"
#include "fileOpt.h"
#include "spj.h"
#include "bufOpt.h"
#include "pageOpt.h"
#include "segment.h" 
#include "Test.h"

class MainWindow;


struct dbMapTable {
	bool isdeleted;
	long logicID;
	long pageNo;
	long recordID;
}; 

class SQLsenten
{
public:
	vector<string> key;//关键字：create，column，listnode，
	map<string, vector<string> > value;//参数列表

	SQLsenten();
	vector<string> string2vec(string line);//按逗号分割字符串

	void print();//打印结果
};

/*为了实现变长记录而增设的，待实现*/
//struct recordHead{
//    int recordLength;
//    int VariableFieldOffset[ATTRIBUTE_NUM];     //变长字段的偏移
//    
//}; 

struct SysDesc{ 
	struct FileDesc fileDesc[MAX_FILE_NUM];		//	对每一个文件进行描述

	long sizeOfFile;                            //	文件中数据区的大小，默认设为196MB
	long sizePerPage;                           //	每一个页的大小，默认4KB
	long TotalPage;                             //	总共的页数
	long AvaiPage;                              //	当前有多少可用的页

	//空闲空间位示图
	long addr_FreeSpace_BitMap;                    //	文件中bitMap的起始地址
	long size_FreeSpace_BitMap;                   //	bitMap的大小，以字节为单位

	//空闲逻辑号位示图
	long addr_FreeLogicID_BitMap;
	long size_FreeLogicID_BitMap;

	long addr_data;                              //  文件中数据区的起始位置
	int fid_MapTable;                           //("逻辑号-页号记录号")映射表的文件号
	int fid_DataDictionary;                       //存储数据库中各个表模式的文件号（用于将data_dict数组写回文件中）
	long curRecordNum;                          //当前数据库中的记录总数

	int curFileNum;                            //	目前有多少个文件，最多为 MAX_FILE_NUM
};

struct dbSysHead{
	struct SysDesc desc;
	unsigned long *FreeSpace_bitMap;
	unsigned long *FreeLogicID_bitMap;
	struct buffSpace buff;
	Relation data_dict[MAX_FILE_NUM];       //关系数据字典，先存在这里，最后关闭数据库时要刷回到文件中
	FILE *fpdesc;

	map<string, int> tableMap;
	vector<string> tableName;
};

//更新，把内存中的所有数据写入到磁盘中
int memToDisk(struct dbSysHead *head);

class Storage
{
private:
	vector<std::vector<supplier*>>		m_suppliers;

public:
	Storage();
	~Storage();

	void creatSysHead(char *fileName, MainWindow* ui);
	void init_database(struct dbSysHead *head, char *fileName, MainWindow *ui);
	int readDataDictionaryFile(struct dbSysHead *head);
	void show_MapTableFile(struct dbSysHead *head, int fid, MainWindow* ui);
	void show_SysDesc(struct dbSysHead *head, MainWindow* ui);
	void init_MapData(struct dbSysHead *head, MainWindow* ui);
	int init_Table(struct dbSysHead *head, MainWindow* ui);
	void show_Relation(struct dbSysHead *head, int dictID, MainWindow* ui);
	void show_FileDesc(struct dbSysHead *head, int fid, MainWindow* ui);
	void showTables(struct dbSysHead *head, MainWindow* ui);

	//closeDB.cpp
	int sysUpdate(struct dbSysHead *head);
	void close_database(struct dbSysHead* head, MainWindow* ui);
	void deleteFile(struct dbSysHead *head, int fid);
	void deleteDataDict(struct dbSysHead *head, int fid);

	void welcome(MainWindow* ui);
	void exitdb(MainWindow* ui);

	FileOpt fileOpt;
	BufOpt bufOpt;
	PageOpt pageOpt;
	SPJ spjOpt;
}; 
////initial.cpp
//void createSysHead(char *filename);
//void init_database(struct dbSysHead *head, char *filename);
//int readDataDictionaryFile(struct dbSysHead *head);
//
////show.cpp
//void show_SysDesc(struct dbSysHead *head);
//void show_Relation(struct dbSysHead *head, int dictID);
//void show_FileDesc(struct dbSysHead *head, int fid);
//void show_MapTableFile(struct dbSysHead *head, int fid);
//void welcome();
//void exitdb();
//void changeTime(char *target, char *type);
//
////fileOption.cpp
//int createFile(struct dbSysHead *head, int type, long reqPageNum);
//struct pageHead extendFile(struct dbSysHead *head, int fid, struct pageHead *preph);
//void writeFile(struct dbSysHead *head, int dictID, char *str);
//void readFile(struct dbSysHead *head, int dictID);
//void writeFile(struct dbSysHead *head, int dictID);
//
////buffManage.cpp
//int queryPage(struct dbSysHead *head, long queryPageNo);
//void replacePage(struct dbSysHead *head, int mapNo, long pageNo);
//int scheBuff(struct dbSysHead *head);
//intbufOpt.reqPage(struct dbSysHead *head, long queryPageNo);
//
////pageOption.cpp
//int getBit(unsigned long num, long pos);
//int setBit(unsigned long *num, long pos, int setValue);
//long allocatePage(struct dbSysHead *head, long reqPageNum);
//void recyOnePage(struct dbSysHead *head, long pageNo);
//void recyAllPage(struct dbSysHead *head);
//
////recordOption.cpp
//int queryRecordByLogicID(struct dbSysHead *head, long logicID, char* des);
//long allocateLogicID(struct dbSysHead *head);
//void recyOneLogicID(struct dbSysHead *head, long logicID);
//void recyAllFreeLogicID(struct dbSysHead *head);
//long getNextRecord(struct dbSysHead *head, int mapNo, int recordNo, char *des);
//
////tableOption.cpp
//int createTable_employee(struct dbSysHead *head);
//int createTable_employee(struct dbSysHead *head, string sql);
//int createTable_department(struct dbSysHead *head);
//int createTable_birthday(struct dbSysHead *head);
//int createTable_supplier(struct dbSysHead *head);
//int createTable_partsupp(struct dbSysHead *head);
////void dropTable(struct dbSysHead *head, char* tableName);
//
////select.cpp
//int tableScanEqualSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* value);
//int tableScanRangeSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* min, char* max);
//
////projection.cpp
//int projection(struct dbSysHead *head, int dictID, char* attribute_name);
//int getValueByAttrID(char *str, int index, char *result);
//
////join.cpp
//int nestedLoopJoin(struct dbSysHead *head, int employee_dictID, int department_dictID);
//int nestedLoopJoinByConds(struct dbSysHead *head, int employee_dictID, int department_dictID, char* attribute_name, char* value, int like);
//int nestedLoopJoinByThree(struct dbSysHead *head, int employee_dictID, int department_dictID, int birthday_dictID);
//int SortJoin(struct dbSysHead *head, int employee_dictID, int department_dictID);
//int HashJoin(struct dbSysHead *head, int employee_dictID, int department_dictID);
//
//
////tmpTable.cpp
//int createTmpTable(struct dbSysHead *head, Relation original_rl);
//int createTmpTable2(struct dbSysHead *head, Relation r1, Relation r2, int r1_puc_attr, int r2_puc_attr);
//int createTmpTable3(struct dbSysHead *head, Relation r1, Relation r2, Relation r3, int r1_pub_attr, int r2_pub_attr, int r3_pub_attr);
//int createTmpTableAfterSort(struct dbSysHead *head, Relation rl, int pub_attr);
//void HashRelation(struct dbSysHead *head, Relation rl, int pub_attr, multimap<int, long> *m);
//int insertOneRecord(struct dbSysHead *head, int tmp_table_dictID, char* record);
//
//
////closeDB.cpp
//int sysUpdate(struct dbSysHead *head);
//void close_database(struct dbSysHead* head);
//void deleteFile(struct dbSysHead *head, int fid);
//void deleteDataDict(struct dbSysHead *head, int fid);
//
////sqlUtil.cpp
//void createSql(string sql_str, string& table_name, map<string, pair<int, int> >& paras_map);


#endif  // DBHEAD_H_INCLUDED