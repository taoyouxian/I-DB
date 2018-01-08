#pragma once

#ifndef SPJ_H_INCLUDED
#define SPJ_H_INCLUDED

#include "common.h"

class SPJ
{
public:
	SPJ();
	~SPJ();

	long queryLogicID(struct dbSysHead *head, long logicID, struct pageHead *PHead);
	//recordOption.cpp
	int queryRecordByLogicID(struct dbSysHead *head, long logicID, char* des);
	long allocateLogicID(struct dbSysHead *head);
	void recyOneLogicID(struct dbSysHead *head, long logicID);
	void recyAllFreeLogicID(struct dbSysHead *head);
	long getNextRecord(struct dbSysHead *head, int mapNo, int recordNo, char *des);
	 
	// 给定逻辑号，更新该记录的内容
	void updateRecordByLogicID(struct dbSysHead *head, long logicID, char* str);
	//索引部分
	void queryRecordByKey(struct dbSysHead *head, int fid, int key, char *des);
	void insertRecordByKey(struct dbSysHead *head, int fid, char *str);
	void deleteRecordByKey(struct dbSysHead *head, int fid, int key);


	//tableOption.cpp
	int createTable_employee(struct dbSysHead *head);
	int createTable_employee(struct dbSysHead *head, string sql);
	int createTable_department(struct dbSysHead *head);
	int createTable_birthday(struct dbSysHead *head);
	int createTable_supplier(struct dbSysHead *head);
	int createTable_partsupp(struct dbSysHead *head);
	int createTable_nation(struct dbSysHead *head);
	int createTable(struct dbSysHead *head, vector<std::string> vv, string tablename);
	//void dropTable(struct dbSysHead *head, char* tableName);

	//select.cpp
	int tableScanCountSelector(struct dbSysHead *head, int dictID);
	int tableScanEqualSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* compType, char* value);
	int tableScanRangeSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* min, char* max);

	int tableScanCountSelectorByCond(struct dbSysHead *head, int dictID, char* attribute_name, char* compType, char* value);
	//projection.cpp
	int projection(struct dbSysHead *head, int dictID, char* attribute_name);
	int getValueByAttrID(char *str, int index, char *result);

	//join.cpp
	int nestedLoopJoin(struct dbSysHead *head, int employee_dictID, int department_dictID);
	int nestedLoopJoinByConds(struct dbSysHead *head, int employee_dictID, int department_dictID, char* attribute_name, char* value, int like);
	int nestedLoopJoinByThree(struct dbSysHead *head, int employee_dictID, int department_dictID, int birthday_dictID);
	int SortJoin(struct dbSysHead *head, int employee_dictID, int department_dictID);
	int HashJoin(struct dbSysHead *head, int employee_dictID, int department_dictID);

	//tmpTable.cpp
	int createTmpTable(struct dbSysHead *head, Relation original_rl);
	int createTmpTable2(struct dbSysHead *head, Relation r1, Relation r2, int r1_puc_attr, int r2_puc_attr);
	int createTmpTable3(struct dbSysHead *head, Relation r1, Relation r2, Relation r3, int r1_pub_attr, int r2_pub_attr, int r3_pub_attr);
	int createTmpTableAfterSort(struct dbSysHead *head, Relation rl, int pub_attr);
	void HashRelation(struct dbSysHead *head, Relation rl, int pub_attr, multimap<int, long> *m);
	int insertOneRecord(struct dbSysHead *head, int tmp_table_dictID, char* record);


	//closeDB.cpp
	int sysUpdate(struct dbSysHead *head);
	void close_database(struct dbSysHead* head);
	void deleteFile(struct dbSysHead *head, int fid);
	void deleteDataDict(struct dbSysHead *head, int fid);

};


#endif // SPJ_H_INCLUDED