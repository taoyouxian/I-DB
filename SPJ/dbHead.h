#ifndef dbHead_h
#define dbHead_h

#include "common.h"


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
void changeTime(char *target, char *type);

//fileOption.cpp
int createFile(struct dbSysHead *head, int type, long reqPageNum);
struct pageHead extendFile(struct dbSysHead *head, int fid, struct pageHead *preph);
void writeFile(struct dbSysHead *head, int dictID, char *str);
void readFile(struct dbSysHead *head, int dictID);
void writeFile(struct dbSysHead *head, int dictID);

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
int createTable_birthday(struct dbSysHead *head);
//void dropTable(struct dbSysHead *head, char* tableName);

//select.cpp
int tableScanEqualSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* value);
int tableScanRangeSelector(struct dbSysHead *head, int dictID, char* attribute_name, char* min, char* max);

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


#endif /* dbHead_h */
