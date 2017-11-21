#ifndef dbHead_h
#define dbHead_h

#include "common.h"

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
#define DATE_TYPE       3

#define NAME_MAX_LENGTH 32
#define ATTRIBUTE_NUM   12

#define BUCKET_NUM      10


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
using namespace std;

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
};


struct dbMapTable {
	bool isdeleted;
	long logicID;
	long pageNo;
	long recordID;
};

struct buffBlock{
	long pageNo;
	long visitTime;
	bool isEdited;
	bool isPin;
};

struct buffSpace{
	struct buffBlock map[SIZE_BUFF];
	char data[SIZE_BUFF][SIZE_PER_PAGE];
	long curTimeStamp;
};

/* 块内的偏移量表从块的前端向后增长
* 块内的记录是从后向前放置
*/
struct offsetInPage{
	long logicID;       // 该记录在db中的逻辑号（作为逻辑地址）
	int recordID;       // 该记录在当前页的记录号
	int offset;         //该记录相对于块尾地址的偏移量
	bool isDeleted;
};

/*为了实现变长记录而增设的，待实现*/
//struct recordHead{
//    int recordLength;
//    int VariableFieldOffset[ATTRIBUTE_NUM];     //变长字段的偏移
//    
//};
struct pageHead{
	long pageNo;                    // 页号
	int curRecordNum;               // 当前该页存储的记录个数
	long prePageNo;
	long nextPageNo;
	long freeSpace;                 // 该页的空余空间大小
};

struct FileDesc{
	int fileType;                  //标识文件类型
	//int fileID;                    //文件号
	long fileFirstPageNo;
	long filePageNum;              //文件占用了多少页
};

struct SysDesc{

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
	struct FileDesc fileDesc[MAX_FILE_NUM];		//	对每一个文件进行描述
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
	char attributeName[NAME_MAX_LENGTH];//属性名
	int type;                           //属性类型
	int length;                         //属性长度
	int recordDeviation;                //记录内偏移（在关系属性表中的偏移）
};


class Relation {
public:
	int initRelation(struct dbSysHead *head, int fid, const char *relationName);
	int insertAttribute(char *name, int type, int length);
	Attribute getAttributeByName(char *name);
	int getAttributeIndexByName(char *name);

	int fileID;                          //该关系模式对应的文件号
	char relationName[NAME_MAX_LENGTH];   //关系名
	int attributeNum;                     //属性个数
	Attribute atb[ATTRIBUTE_NUM];         //属性表
	bool isIndexed[ATTRIBUTE_NUM];        //是否在对应属性上建立了索引
	bool isOrdered[ATTRIBUTE_NUM];        //是否在对应属性上有序
	int recordLength;
	int recordNum;
};

struct dbSysHead{
	struct SysDesc desc;
	unsigned long *FreeSpace_bitMap;
	unsigned long *FreeLogicID_bitMap;
	struct buffSpace buff;
	Relation data_dict[MAX_FILE_NUM];       //关系数据字典，先存在这里，最后关闭数据库时要刷回到文件中
	FILE *fpdesc;
};

//initial.cpp
void createSysHead(char *filename);
void init_database(struct dbSysHead *head, char *filename);
int readDataDictionaryFile(struct dbSysHead *head);

//show.cpp
void show_SysDesc(struct dbSysHead *head);
void show_Relation(struct dbSysHead *head, int dictID);
void show_FileDesc(struct dbSysHead *head, int fid);
void show_MapTableFile(struct dbSysHead *head, int fid);
void welcome();
void exitdb();

//fileOption.cpp
int createFile(struct dbSysHead *head, int type, long reqPageNum);
struct pageHead extendFile(struct dbSysHead *head, int fid, struct pageHead *preph);
void writeFile(struct dbSysHead *head, int dictID, char *str);
void readFile(struct dbSysHead *head, int dictID);

//buffManage.cpp
int queryPage(struct dbSysHead *head, long queryPageNo);
int replacePage(struct dbSysHead *head, int mapNo, long pageNo);
int scheBuff(struct dbSysHead *head);
int reqPage(struct dbSysHead *head, long queryPageNo);

//pageOption.cpp
int getBit(unsigned long num, long pos);
int setBit(unsigned long *num, long pos, int setValue);
long allocatePage(struct dbSysHead *head, long reqPageNum);
void recyOnePage(struct dbSysHead *head, long pageNo);
void recyAllPage(struct dbSysHead *head);

//recordOption.cpp
int queryRecordByLogicID(struct dbSysHead *head, long logicID, char* des);
long allocateLogicID(struct dbSysHead *head);
void recyOneLogicID(struct dbSysHead *head, long logicID);
void recyAllFreeLogicID(struct dbSysHead *head);
long getNextRecord(struct dbSysHead *head, int mapNo, int recordNo, char *des);

//tableOption.cpp
int createTable_employee(struct dbSysHead *head);
int createTable_department(struct dbSysHead *head);
//void dropTable(struct dbSysHead *head, char* tableName);

//select.cpp
int tableScanEqualSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* value);
int tableScanRangeSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* min, char* max);

//projection.cpp
int projection(struct dbSysHead *head, int dictID, char* attribute_name);
int getValueByAttrID(char *str, int index, char *result);

//join.cpp
int nestedLoopJoin(struct dbSysHead *head, int employee_dictID, int department_dictID);
int SortJoin(struct dbSysHead *head, int employee_dictID, int department_dictID);
int HashJoin(struct dbSysHead *head, int employee_dictID, int department_dictID);


//tmpTable.cpp
int createTmpTable(struct dbSysHead *head, Relation original_rl);
int createTmpTable2(struct dbSysHead *head, Relation r1, Relation r2, int r1_puc_attr, int r2_puc_attr);
int createTmpTableAfterSort(struct dbSysHead *head, Relation rl, int pub_attr);
void HashRelation(struct dbSysHead *head, Relation rl, int pub_attr, multimap<int, long> *m);
int insertOneRecord(struct dbSysHead *head, int tmp_table_dictID, char* record);


//closeDB.cpp
int sysUpdate(struct dbSysHead *head);
void close_database(struct dbSysHead* head);
void deleteFile(struct dbSysHead *head, int fid);
void deleteDataDict(struct dbSysHead *head, int fid);

#endif /* dbHead_h */
