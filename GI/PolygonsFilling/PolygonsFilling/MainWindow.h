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
	shared_ptr<Table>			m_QtableWin;			//	�½�����

private:
	Ui::GLWidgetClass ui;
	 
    QWidget *centralWidget;
    GLWidget *glWidget;
	  
	// ����������
	void	CreateToolBar();
	// ��������
	void	CreateView();
	// ��ʼ����ʾ��
	void	InitViewWidget();

	int CopyFile(char *SourceFile, char *NewFile); 

public:
	QToolBar	*m_pSavaAsFileToolBar;		// ����Ϊ�ļ�
	QAction		*m_pSavaAsFileAction;		

	QToolBar	*m_pOpenFileToolBar;		// ���ļ�
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

/**�ۺ���**/ 
public:
	// �ļ���ȡ
	void	ReadFiletoPoints();
	void	NewTable();
	void	BacktoMain();
	void	Query();
	// �ļ�д��
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
	// ����ҳ������������
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