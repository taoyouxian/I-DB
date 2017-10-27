#pragma once

#ifndef FILEOPT_H_INCLUDED
#define FILEOPT_H_INCLUDED

#include "common.h"
#include "pageOpt.h"
#include "segmentOpt.h"
#include "bufOpt.h"
#include "Test.h"
 
#include <vector>
#include <QString> 

/* (文件信息)
*/
struct FileInfo{
	int fileType;                // 标识是否为索引文件，以及索引文件的类型（hash/b树/顺序,线性）
	int fileID;                  // 文件号
	int fileName;                // 文件名
	int fileState;                // 文件状态
	int fileSegNum;              // 文件占用了多少段
	struct Segment segTable[SEG_NUM];		//	定义段数量
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


	int createFile(struct DbMetaHead *head, int type, long requestPageNum, MainWindow* ui);
	void readFile(struct DbMetaHead *head, int fileID, char *des, MainWindow* ui);
	void readFile(struct DbMetaHead *head, int fileID, char *des, MainWindow* ui, int pageNo);
	void writeFile(struct DbMetaHead *head, int fileID, int length, char *str, MainWindow* ui);
	void deleteFile(struct DbMetaHead *head, int fileID, MainWindow* ui);
	int queryFileByIndex(struct DbMetaHead *head, int fileID, MainWindow* ui);

	PageOpt pageOpt;
	BufOpt bufOpt;

private:
	QString		OpenFileDialog();
	QString		SaveFileDialog();
	vector<std::string>		InfoSplit(const string& s, const string& c);
};


#endif // PAGEOPT_H_INCLUDED