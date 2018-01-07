//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets\QMainWindow>
#include <memory>
#include "ui_glwidget.h"
#include "ui_table.h" 
#include "storage.h" 
#include "Table.h"

#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QButtonGroup>
class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
	~MainWindow();

signals:
	void SGNewTable();

private: 
	shared_ptr<Table>			m_QtableWin;			//	新建窗口

private:
	Ui::GLWidgetClass ui;
	 
    QWidget *centralWidget;
    GLWidget *glWidget;
	  
	// 创建工具栏
	void	CreateToolBar();
	// 创建区域
	void	CreateView();
	// 初始化显示区
	void	InitViewWidget();

	int CopyFile(char *SourceFile, char *NewFile); 

public:
	QToolBar	*m_pSavaAsFileToolBar;		// 保存为文件
	QAction		*m_pSavaAsFileAction;		

	QToolBar	*m_pOpenFileToolBar;		// 打开文件
	QAction		*m_pOpenFileAction;	 

	QToolBar	*m_pNewToolBar;			// NewTable
	QAction		*m_pNewAction;

	QToolBar	*m_pQueryToolBar;			// Query
	QAction		*m_pQueryAction;

	QToolBar	*m_pInitToolBar;			// InitDB
	QAction		*m_pInitAction;
	QToolBar	*m_pDeleteFileToolBar;			// DeleteFile
	QAction		*m_pDeleteFileAction;
	QToolBar	*m_pCreateFileToolBar;			// CreateFile
	QAction		*m_pCreateFileAction;
	QToolBar	*m_pReadFileToolBar;			// ReadFile
	QAction		*m_pReadFileAction;
	QToolBar	*m_pMemToDiskToolBar;			// MemToDisk
	QAction		*m_pMemToDiskAction;
	QToolBar	*m_pShowDBInfoToolBar;			// ShowDBInfo
	QAction		*m_pShowDBInfoAction;
	QToolBar	*m_pLoadDataToolBar;			// LoadData
	QAction		*m_pLoadDataAction;
	QToolBar	*m_pBackupToolBar;			// Backup
	QAction		*m_pBackupAction; 
	QToolBar	*m_pFileDescToolBar;			// Free
	QAction		*m_pFileDescAction;


	QTextBrowser		*sysBrowser;
	QTextBrowser		*sysTables;
	QTextBrowser		*showBrowser;
	QTextBrowser		*tBrowser;
	QTextBrowser		*tQuery;
	QTextEdit		*sqlText;
	QTextEdit		*loadText;
	QLineEdit		*searchText;

	QPushButton		*queryBtn;
	QPushButton		*loadBtn;
	QPushButton		*searchBtn;
	QPushButton		*initBtn;
	QPushButton		*delBtn;
	QPushButton		*createFileBtn;
	QPushButton		*memToDiskBtn;
	QPushButton		*showDBInfoBtn;
	QPushButton		*backupBtn;
	QPushButton		*loadDataBtn;


	QTableWidget		*tableWidget;

/**槽函数**/ 
public:
	// 文件读取
	void	ReadFiletoPoints();
	void	NewTable();
	void	BacktoMain();
	void	Query();
	// 文件写入
	void	SavePolyAsFile();

	void    InitDB();
	void	DeleteFile();
	void	CreateFile();
	void	ReadFile();
	void	MemToDisk();
	void	ShowDBInfo();
	void	LoadData();
	void	Backup();
	void	ShowFileDesc();
	// 按照页号来检索内容
	void SearchByPageNo();
	void QueryBySQL();
	void LoadByTableName();

	FileOpt fileOpt;
	Storage StorageManager;
	dbSysHead dbhead;

	int supplier_dictID;
	int partsupp_dictID;

	char fileName[15];

	string fileLog;
	string welcomeLog;

	int INFO_NUM = 0; 
	int fileID;
};

#endif // MAINWINDOW_H