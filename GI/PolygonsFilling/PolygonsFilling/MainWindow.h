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
	  
	// ����������
	void	CreateToolBar();
	// ��������
	void	CreateView();
	// ��ʼ����ʾ��
	void	InitViewWidget();

public:
	QToolBar	*m_pSavaAsFileToolBar;		// ����Ϊ�ļ�
	QAction		*m_pSavaAsFileAction;		

	QToolBar	*m_pOpenFileToolBar;		// ���ļ�
	QAction		*m_pOpenFileAction;	 

	QToolBar	*m_pDeleteToolBar;			// ������
	QAction		*m_pDeleteAction;

	QToolBar	*m_pCreateToolBar;			// �������
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

/**�ۺ���**/ 
public:
	// �ļ���ȡ
	void	ReadFiletoPoints();
	// �������
	void	CreateGLWidget();
	// ������
	void	DeleteGLWidget();
	// �ļ�д��
	void	SavePolyAsFile();

	void    InitDB();
	void	DeleteFile();
	void	CreateFile();
	void	MemToDisk();
	void	ShowDBInfo();
	void	LoadData();
	void	Backup();
	// ����ҳ������������
	void SearchByPageNo();
	 
	FileOpt fileOpt;
	 
	Storage StorageManager;
	DbMetaHead dbhead;
	char fileName[15];
	
};

#endif // MAINWINDOW_H