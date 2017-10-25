//mainwindow.cpp
#include <QtOpenGL\QtOpenGL>

#include <QtWidgets\QAction>
#include <QtWidgets\QMenu>
#include <QtWidgets\QScrollArea>
#include <QtWidgets\QMenuBar> 
#include <QtWidgets\QApplication>

#include "mainwindow.h"
#include "glwidget.h"

#include<vector>
#include<list>
using namespace std;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{   
	//ui.setupUi(this); 
    InitViewWidget();
	CreateToolBar();
	CreateView();
}


MainWindow::~MainWindow()
{ 
}

void MainWindow::InitViewWidget()
{
	centralWidget = new QWidget;
	setCentralWidget(centralWidget);

	glWidget = new GLWidget;

	QGridLayout *centralLayout = new QGridLayout;
	centralLayout->addWidget(glWidget, 0, 0);

	centralWidget->setLayout(centralLayout);

	setWindowTitle(QString::fromLocal8Bit("Tiny Database —— Principle and Realization of Database Management System"));
	resize(800, 600);
}

void MainWindow::CreateToolBar()
{
	m_pOpenFileAction = new QAction(QString::fromLocal8Bit("Open"), this);
	connect(m_pOpenFileAction, &QAction::triggered, this, &MainWindow::ReadFiletoPoints);
	m_pOpenFileToolBar = addToolBar(tr("&open"));
	m_pOpenFileToolBar->addAction(m_pOpenFileAction);
	m_pOpenFileToolBar->setAttribute(Qt::WA_DeleteOnClose);

	m_pSavaAsFileAction = new QAction(QString::fromLocal8Bit("Save"), this);
	connect(m_pSavaAsFileAction, &QAction::triggered, this, &MainWindow::SavePolyAsFile);
	m_pSavaAsFileToolBar = addToolBar(tr("&save"));
	m_pSavaAsFileToolBar->addAction(m_pSavaAsFileAction);
	m_pSavaAsFileToolBar->setAttribute(Qt::WA_DeleteOnClose);

	/*m_pCreateAction = new QAction(QString::fromLocal8Bit("Create"), this);
	connect(m_pCreateAction, &QAction::triggered, this, &MainWindow::CreateGLWidget);
	m_pCreateToolBar = addToolBar(tr("create"));
	m_pCreateToolBar->addAction(m_pCreateAction);
	m_pCreateToolBar->setAttribute(Qt::WA_DeleteOnClose);

	m_pDeleteAction = new QAction(QString::fromLocal8Bit("Delete"), this);
	connect(m_pDeleteAction, &QAction::triggered, this, &MainWindow::DeleteGLWidget);
	m_pDeleteToolBar = addToolBar(tr("delete"));
	m_pDeleteToolBar->addAction(m_pDeleteAction);
	m_pDeleteToolBar->setAttribute(Qt::WA_DeleteOnClose);*/

	m_pInitAction = new QAction(QString::fromLocal8Bit("InitDB"), this);
	connect(m_pInitAction, &QAction::triggered, this, &MainWindow::InitDB);
	m_pInitToolBar = addToolBar(tr("InitDB"));
	m_pInitToolBar->addAction(m_pInitAction);
	m_pInitToolBar->setAttribute(Qt::WA_DeleteOnClose); 

	m_pDeleteFileAction = new QAction(QString::fromLocal8Bit("DeleteFile"), this);
	connect(m_pDeleteFileAction, &QAction::triggered, this, &MainWindow::DeleteFile);
	m_pDeleteFileToolBar = addToolBar(tr("DeleteFile"));
	m_pDeleteFileToolBar->addAction(m_pDeleteFileAction);
	m_pDeleteFileToolBar->setAttribute(Qt::WA_DeleteOnClose);

	m_pCreateFileAction = new QAction(QString::fromLocal8Bit("CreateFile"), this);
	connect(m_pCreateFileAction, &QAction::triggered, this, &MainWindow::CreateFile);
	m_pCreateFileToolBar = addToolBar(tr("CreateFile"));
	m_pCreateFileToolBar->addAction(m_pCreateFileAction);
	m_pCreateFileToolBar->setAttribute(Qt::WA_DeleteOnClose);

	m_pMemToDiskAction = new QAction(QString::fromLocal8Bit("MemToDisk"), this);
	connect(m_pMemToDiskAction, &QAction::triggered, this, &MainWindow::MemToDisk);
	m_pMemToDiskToolBar = addToolBar(tr("MemToDisk"));
	m_pMemToDiskToolBar->addAction(m_pMemToDiskAction);
	m_pMemToDiskToolBar->setAttribute(Qt::WA_DeleteOnClose); 

	m_pShowDBInfoAction = new QAction(QString::fromLocal8Bit("ShowDBInfo"), this);
	connect(m_pShowDBInfoAction, &QAction::triggered, this, &MainWindow::ShowDBInfo);
	m_pShowDBInfoToolBar = addToolBar(tr("ShowDBInfo"));
	m_pShowDBInfoToolBar->addAction(m_pShowDBInfoAction);
	m_pShowDBInfoToolBar->setAttribute(Qt::WA_DeleteOnClose);

	m_pBackupAction = new QAction(QString::fromLocal8Bit("Backup"), this);
	connect(m_pBackupAction, &QAction::triggered, this, &MainWindow::Backup);
	m_pBackupToolBar = addToolBar(tr("Backup"));
	m_pBackupToolBar->addAction(m_pBackupAction);
	m_pBackupToolBar->setAttribute(Qt::WA_DeleteOnClose);

	m_pLoadDataAction = new QAction(QString::fromLocal8Bit("LoadData"), this);
	connect(m_pLoadDataAction, &QAction::triggered, this, &MainWindow::LoadData);
	m_pLoadDataToolBar = addToolBar(tr("LoadData"));
	m_pLoadDataToolBar->addAction(m_pLoadDataAction);
	m_pLoadDataToolBar->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::CreateView() {
	QString info = QString::fromLocal8Bit("【") + "System Message Info" + QString::fromLocal8Bit("】");
	sysBrowser = new QTextBrowser(this);
	sysBrowser->setText(info);
	sysBrowser->setGeometry(QRect(0, 31, 400, 560));   // X, Y, W, H
	sysBrowser->setText(info + "\n- - - - - - - - - - - -\n" + "Nothing show");

	info = QString::fromLocal8Bit("【") + "Loading Data Message Info" + QString::fromLocal8Bit("】");
	tBrowser = new QTextBrowser(this);
	tBrowser->setText(info);
	tBrowser->setGeometry(QRect(400, 200, 400, 400));   // X, Y, W, H
	tBrowser->setText(info + "\n- - - - - - - - - - - - - - -\n" + "Nothing show");
	//tBrowser->show(); 

	searchText = new QLineEdit(this);
	searchText->setObjectName(QStringLiteral("msgLine"));
	searchText->setGeometry(QRect(410, 31, 120, 30));
	QFont font2;
	font2.setPointSize(10);
	searchText->setFont(font2);
	searchText->setText("Input Page No");

	searchBtn = new QPushButton(this);
	searchBtn->setObjectName(QStringLiteral("searchBtn"));
	searchBtn->setGeometry(QRect(540, 31, 70, 30));
	searchBtn->setText("Search");
	connect(searchBtn, &QPushButton::clicked, this, &MainWindow::SearchByPageNo); 
	/*
	QPushButton		*searchBtn;
	QPushButton		*initBtn;
	QPushButton		*delBtn;
	QPushButton		*createFileBtn;
	QPushButton		*memToDiskBtn;
	QPushButton		*showDBInfoBtn;
	QPushButton		*BackupBtn;
	*/ 
	/*initBtn = new QPushButton(this);
	initBtn->setObjectName(QStringLiteral("initBtn"));
	initBtn->setGeometry(QRect(0, 50, 70, 30));
	initBtn->setText("Init");
	connect(initBtn, &QPushButton::clicked, this, &MainWindow::InitDB);*/
}  
  
void MainWindow::CreateGLWidget() {

} 

void MainWindow::DeleteGLWidget() {

} 

void MainWindow::ReadFiletoPoints() {
	vector<std::vector<Point*>>		m_points;
	fileOpt.ReadFiletoPoints(m_points); 
}

void MainWindow::SavePolyAsFile()
{
	vector<std::vector<Point*>>		m_points; 
	fileOpt.SavePolyAsFile(m_points);
}

void MainWindow::SearchByPageNo()
{  
	QString value = searchText->text();
	if (value == "" && value != "Input Page No")
	{ 
		QMessageBox::about(NULL, QString::fromLocal8Bit("Waring"), QString::fromLocal8Bit("SearchAction is not null"));
	}
	else{
		//QMessageBox::about(NULL, QString::fromLocal8Bit("文件"), QString::fromLocal8Bit("Page No: " + value.toStdString));
	} 
}

void MainWindow::InitDB()
{
	//QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromLocal8Bit("InitDB")); 
	strcpy(fileName, "tinydb.mat");
	StorageManager.initDB(&dbhead, fileName, this);
	StorageManager.showDbInfo(&dbhead);  
}
void MainWindow::DeleteFile()
{
	QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromLocal8Bit("DeleteFile"));
}
void MainWindow::CreateFile()
{
	QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromLocal8Bit("CreateFile"));
}
void MainWindow::MemToDisk()
{
	QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromLocal8Bit("MemToDisk"));
}
void MainWindow::ShowDBInfo()
{
	QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromLocal8Bit("ShowDBInfo"));
}
void MainWindow::Backup()
{
	// 更改文件名
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t = localtime(&tt);
	/*printf("%d-%02d-%02d %02d:%02d:%02d\n",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);*/
	char time[50];
	sprintf(time, "%d", t->tm_hour);
	strcat(time, t->tm_min + "");
	strcat(time, t->tm_sec + "");
	strcat(time, ".mat");
	rename(fileName, time);
	QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromLocal8Bit("Backup Complete"));
}
void MainWindow::LoadData()
{
	vector<std::vector<supplier*>> suppliers; 
	fileOpt.LoadData(suppliers);


} 