#pragma once

#ifndef FILEOPT_H_INCLUDED
#define FILEOPT_H_INCLUDED

#include "common.h"
#include "segmentOpt.h"
#include "Test.h"
 
#include <vector>
#include <QString> 

/* (文件信息)
*/
struct FileDesc{
	int fileType;                  //标识文件类型
	//int fileID;                    //文件号
	long fileFirstPageNo;
	long filePageNum;              //文件占用了多少页
};

class MainWindow;

class FileOpt
{
public:
	FileOpt(void);
	~FileOpt(void);

public:
	void		SavePolyAsFile();		//	保存为文件
	void		ReadFiletoPoints();	//  读文件
	void		LoadData(std::vector<supplier*> &suppliers);	//  LoadData


	int createFile(struct dbSysHead *head, int type, long requestPageNum, MainWindow* ui);
	int createFile(struct dbSysHead *head, int type, long requestPageNum);
	//void readFile(struct dbSysHead *head, int fileID, char *des, MainWindow* ui);
	//void readFile(struct dbSysHead *head, int fileID, char *des, MainWindow* ui, int pageNo);
	//void writeFile(struct dbSysHead *head, int fileID, char *str, MainWindow* ui);
	void deleteFile(struct dbSysHead *head, int fileID, MainWindow* ui);
	int queryFileByIndex(struct dbSysHead *head, int fileID, MainWindow* ui); 

	struct pageHead extendFile(struct dbSysHead *head, int fid, struct pageHead *preph);
	void writeFile(struct dbSysHead *head, int dictID, char *str, MainWindow* ui);
	void readFile(struct dbSysHead *head, int dictID, char* attribute_name, MainWindow* ui);
	void deleteRecordByCompKey(struct dbSysHead *head, int dictID, char* attribute_name, char* compType, char* value, MainWindow* ui);
	void deleteRecord(struct dbSysHead *head, int dictID, MainWindow* ui);
	void writeFile(struct dbSysHead *head, int dictID, MainWindow* ui);
	void insertIntoMapFile(struct dbSysHead *head, struct dbMapTable mt, long logicID);

	QString		OpenFileDialog();
	vector<std::string>		InfoSplit(const string& s, const string& c);

private:
	//QString		OpenFileDialog();
	QString		SaveFileDialog();
};


#endif // PAGEOPT_H_INCLUDED