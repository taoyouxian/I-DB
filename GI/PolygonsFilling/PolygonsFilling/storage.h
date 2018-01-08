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
	vector<string> key;//�ؼ��֣�create��column��listnode��
	map<string, vector<string> > value;//�����б�

	SQLsenten();
	vector<string> string2vec(string line);//�����ŷָ��ַ���

	void print();//��ӡ���
};

/*Ϊ��ʵ�ֱ䳤��¼������ģ���ʵ��*/
//struct recordHead{
//    int recordLength;
//    int VariableFieldOffset[ATTRIBUTE_NUM];     //�䳤�ֶε�ƫ��
//    
//}; 

struct SysDesc{ 
	struct FileDesc fileDesc[MAX_FILE_NUM];		//	��ÿһ���ļ���������

	long sizeOfFile;                            //	�ļ����������Ĵ�С��Ĭ����Ϊ196MB
	long sizePerPage;                           //	ÿһ��ҳ�Ĵ�С��Ĭ��4KB
	long TotalPage;                             //	�ܹ���ҳ��
	long AvaiPage;                              //	��ǰ�ж��ٿ��õ�ҳ

	//���пռ�λʾͼ
	long addr_FreeSpace_BitMap;                    //	�ļ���bitMap����ʼ��ַ
	long size_FreeSpace_BitMap;                   //	bitMap�Ĵ�С�����ֽ�Ϊ��λ

	//�����߼���λʾͼ
	long addr_FreeLogicID_BitMap;
	long size_FreeLogicID_BitMap;

	long addr_data;                              //  �ļ�������������ʼλ��
	int fid_MapTable;                           //("�߼���-ҳ�ż�¼��")ӳ�����ļ���
	int fid_DataDictionary;                       //�洢���ݿ��и�����ģʽ���ļ��ţ����ڽ�data_dict����д���ļ��У�
	long curRecordNum;                          //��ǰ���ݿ��еļ�¼����

	int curFileNum;                            //	Ŀǰ�ж��ٸ��ļ������Ϊ MAX_FILE_NUM
};

struct dbSysHead{
	struct SysDesc desc;
	unsigned long *FreeSpace_bitMap;
	unsigned long *FreeLogicID_bitMap;
	struct buffSpace buff;
	Relation data_dict[MAX_FILE_NUM];       //��ϵ�����ֵ䣬�ȴ���������ر����ݿ�ʱҪˢ�ص��ļ���
	FILE *fpdesc;

	map<string, int> tableMap;
	vector<string> tableName;
};

//���£����ڴ��е���������д�뵽������
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