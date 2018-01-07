#pragma once

#ifndef COMMONS_H_INCLUDED
#define COMMONS_H_INCLUDED

#include <iostream>
#include <cstdio>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <algorithm>
#include <vector> 
#include <fstream>
#include <iomanip>
#include <Windows.h>  
#include <map>
#include <sstream>
#include "Test.h"
using namespace std;

#define PATH "H:\\SelfLearning\\SAI\\Course\\tinydbms\\GI\\PolygonsFilling\\"

#define MAX_FILE_NUM 64
#define MAX_LOGICID_NUM 1024000

#define SIZE_PAGEHEAD sizeof(struct pageHead)
#define SIZE_OFFSET   sizeof(struct offsetInPage)

#define SIZE_BUFF 100
#define SIZE_PER_PAGE (4*1024)
#define SIZE_DATA_SPACE (196*1024*1024)
#define SIZE_FREE_SPACE_BIT_MAP  (SIZE_DATA_SPACE/(SIZE_PER_PAGE*8))
//ӳ���Ĵ�С�ɼ�¼������������ʱ��̬������1024000���߼��ţ���1024000bit=128000B=125KB
#define SIZE_FREE_LOGICID_BIT_MAP MAX_LOGICID_NUM/8

#define ADDR_FREE_SPACE_BIT_MAP sizeof(struct SysDesc)
#define ADDR_FREE_LOGICID_BIT_MAP (ADDR_FREE_SPACE_BIT_MAP + SIZE_FREE_SPACE_BIT_MAP)
#define ADDR_DATA (ADDR_FREE_LOGICID_BIT_MAP + SIZE_FREE_LOGICID_BIT_MAP)

#define MAP_FILE         0
#define DATA_DICTIONARY  1
#define USER_FILE        2
#define INDEX_FILE_BTREE 3
#define TMP_TABLE        4
#define HASH_BUCKET      5

#define NORMAL_FILE      1
#define INDEX_FILE_HASH  2

#define ALLO_FAIL     -1
#define PAGE_AVAI      1  //��ҳ���У�����
#define PAGE_UNAVAI    0
#define LOGICID_AVAI   1  //���߼��ſ��У�����
#define LOGICID_UNAVAI 0

#define BUFF_NOT_HIT -1

//���Ե�����
#define INT_TYPE        1
#define CHAR_TYPE       2
#define VARCHAR_TYPE       3
#define DATE_TYPE       4
#define LONG_TYPE       5
#define FLOAT_TYPE       6
#define DOUBLE_TYPE       7

#define NAME_MAX_LENGTH 32
#define ATTRIBUTE_NUM   12

#define BUCKET_NUM      10



struct employee{
	int rid;    //ְ����
	int did;    //���ڲ��ŵ�id
	int age;
	char rname[20];
};

struct department{
	int did;            //����id
	int manager_id;     //���ž���ְ����
	int num;            //����������
	char dname[20];     //��������
	char idcard[18];    //���ž������֤
};

struct birthday{
	char idcard[18];    //���ž������֤
	char birthtime[15];    //���ž������ʱ��
	int addressid;    //���ž��������ַ, ��ʾ������ַ�����е��±�
};


class Attribute {
public:
	int initAttribute(char *name, int type, int length, int deviation);
	char *getName();
	int getType();
	int getLength();
	int getRecordDeviation();
	bool isSameAttribute(Attribute a);

private:
	char attributeName[NAME_MAX_LENGTH];//������
	int type;                           //��������
	int length;                         //���Գ���
	int recordDeviation;                //��¼��ƫ�ƣ��ڹ�ϵ���Ա��е�ƫ�ƣ�
};


class Relation {
public:
	int initRelation(struct dbSysHead *head, int fid, const char *relationName);
	int insertAttribute(char *name, int type, int length);
	Attribute getAttributeByName(char *name);
	int getAttributeIndexByName(char *name);

	int fileID;                          //�ù�ϵģʽ��Ӧ���ļ���
	char relationName[NAME_MAX_LENGTH];   //��ϵ��
	int attributeNum;                     //���Ը���
	Attribute atb[ATTRIBUTE_NUM];         //���Ա�
	bool isIndexed[ATTRIBUTE_NUM];        //�Ƿ��ڶ�Ӧ�����Ͻ���������
	bool isOrdered[ATTRIBUTE_NUM];        //�Ƿ��ڶ�Ӧ����������
	int recordLength;
	int recordNum;
};


class commons
{
public:
	commons();
	~commons();
};
 
void changeTime(char *target, char *type);

//sqlUtil.cpp
void createSql(string sql_str, string& table_name, map<string, pair<int, int> >& paras_map);

#endif // COMMONS_H_INCLUDED
