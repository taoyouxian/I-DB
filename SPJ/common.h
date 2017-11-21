#pragma once

#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <iostream>
#include <cstdio>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <vector> 
#include <fstream>
#include <map>
using namespace std;

#define PATH "H:\\SelfLearning\\SAI\\Course\\tinydbms\\SPJAlgorithm\\"

#define MAX_FILE_NUM 64
#define MAX_LOGICID_NUM 1024000

#define SIZE_PAGEHEAD sizeof(struct pageHead)
#define SIZE_OFFSET   sizeof(struct offsetInPage)

#define SIZE_BUFF 100
#define SIZE_PER_PAGE (4*1024)
#define SIZE_DATA_SPACE (196*1024*1024)
#define SIZE_FREE_SPACE_BIT_MAP  (SIZE_DATA_SPACE/(SIZE_PER_PAGE*8))
//映射表的大小由记录个数决定，暂时静态分配其1024000个逻辑号，即1024000bit=128000B=125KB
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
//#define INDEX_FILE_HASH  4
//#define INDEX_FILE_SEQ   5

#define ALLO_FAIL     -1
#define PAGE_AVAI      1  //该页空闲，可用
#define PAGE_UNAVAI    0
#define LOGICID_AVAI   1  //该逻辑号空闲，可用
#define LOGICID_UNAVAI 0

#define BUFF_NOT_HIT -1

//属性的类型
#define INT_TYPE        1
#define CHAR_TYPE       2
#define VARCHAR_TYPE       3
#define DATE_TYPE       4
#define FLOAT_TYPE       5
#define DOUBLE_TYPE       6

#define NAME_MAX_LENGTH 32
#define ATTRIBUTE_NUM   12

#define BUCKET_NUM      10



struct employee{
	int rid;    //职工号
	int did;    //所在部门的id
	int age;
	char rname[20];
};

struct department{
	int did;            //部门id
	int manager_id;     //部门经理职工号
	int num;            //部门总人数
	char dname[20];     //部门名字
	char idcard[18];    //部门经理身份证
};

struct birthday{
	char idcard[18];    //部门经理身份证
	char birthtime[15];    //部门经理出生时间
	int addressid;    //部门经理出生地址, 表示出生地址数组中的下标
};


class common
{
public:
	common();
	~common();
};


#endif // COMMON_H_INCLUDED