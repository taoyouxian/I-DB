//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets\QMainWindow>
#include "ui_glwidget.h"
#include "storage.h" 

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

public:
	QToolBar	*m_pSavaAsFileToolBar;		// 保存为文件
	QAction		*m_pSavaAsFileAction;		

	QToolBar	*m_pOpenFileToolBar;		// 打开文件
	QAction		*m_pOpenFileAction;	 

	QToolBar	*m_pDeleteToolBar;			// 清除面板
	QAction		*m_pDeleteAction;

	QToolBar	*m_pCreateToolBar;			// 填充多边形
	QAction		*m_pCreateAction;

	QToolBar	*m_pInitToolBar;			// InitDB
	QAction		*m_pInitAction;
	QToolBar	*m_pDeleteFileToolBar;			// DeleteFile
	QAction		*m_pDeleteFileAction;
	QToolBar	*m_pCreateFileToolBar;			// CreateFile
	QAction		*m_pCreateFileAction;
	QToolBar	*m_pMemToDiskToolBar;			// MemToDisk
	QAction		*m_pMemToDiskAction;
	QToolBar	*m_pShowDBInfoToolBar;			// ShowDBInfo
	QAction		*m_pShowDBInfoAction;
	QToolBar	*m_pLoadDataToolBar;			// LoadData
	QAction		*m_pLoadDataAction;
	QToolBar	*m_pBackupToolBar;			// Backup
	QAction		*m_pBackupAction;


	QTextBrowser		*sysBrowser;

	QTextBrowser		*tBrowser;
	QLineEdit		*searchText;

	QPushButton		*searchBtn;
	QPushButton		*initBtn;
	QPushButton		*delBtn;
	QPushButton		*createFileBtn;
	QPushButton		*memToDiskBtn;
	QPushButton		*showDBInfoBtn;
	QPushButton		*backupBtn;
	QPushButton		*loadDataBtn;

/**槽函数**/ 
public:
	// 文件读取
	void	ReadFiletoPoints();
	// 填充多边形
	void	CreateGLWidget();
	// 清除面板
	void	DeleteGLWidget();
	// 文件写入
	void	SavePolyAsFile();

	void    InitDB();
	void	DeleteFile();
	void	CreateFile();
	void	MemToDisk();
	void	ShowDBInfo();
	void	LoadData();
	void	Backup();
	// 按照页号来检索内容
	void SearchByPageNo();
	 
	FileOpt fileOpt;
	 
	Storage StorageManager;
	DbMetaHead dbhead;
	char fileName[15];
	
};

#endif // MAINWINDOW_H