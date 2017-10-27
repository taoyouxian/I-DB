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

	m_pLoadDataAction = new QAction(QString::fromLocal8Bit("LoadData"), this);
	connect(m_pLoadDataAction, &QAction::triggered, this, &MainWindow::LoadData);
	m_pLoadDataToolBar = addToolBar(tr("LoadData"));
	m_pLoadDataToolBar->addAction(m_pLoadDataAction);
	m_pLoadDataToolBar->setAttribute(Qt::WA_DeleteOnClose);

	m_pReadFileAction = new QAction(QString::fromLocal8Bit("ReadFile"), this);
	connect(m_pReadFileAction, &QAction::triggered, this, &MainWindow::ReadFile);
	m_pReadFileToolBar = addToolBar(tr("ReadFile"));
	m_pReadFileToolBar->addAction(m_pReadFileAction);
	m_pReadFileToolBar->setAttribute(Qt::WA_DeleteOnClose);

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

	m_pFreeAction = new QAction(QString::fromLocal8Bit("Free"), this);
	connect(m_pFreeAction, &QAction::triggered, this, &MainWindow::FreeData);
	m_pFreeToolBar = addToolBar(tr("Free"));
	m_pFreeToolBar->addAction(m_pFreeAction);
	m_pFreeToolBar->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::CreateView() {
	QString info = QString::fromLocal8Bit("【") + "System Message Info" + QString::fromLocal8Bit("】");
	sysBrowser = new QTextBrowser(this);
	sysBrowser->setText(info);
	sysBrowser->setGeometry(QRect(0, 31, 400, 560));   // X, Y, W, H
	sysBrowser->setText(info + "\n- - - - - - - - - - - -\n\n" + "Nothing show");

	info = QString::fromLocal8Bit("【") + "Show The Demo Info" + QString::fromLocal8Bit("】");
	showBrowser = new QTextBrowser(this);
	showBrowser->setText(info);
	showBrowser->setGeometry(QRect(400, 31, 400, 100));   // X, Y, W, H
	showBrowser->setText(info + "\n- - - - - - - - - - - - - - -\n\n" + "initDB -> deleteFile -> createFile -> memToDisk -> writeFile(loadData) -> readFile -> memToDisk, free(showDbInfo)");

	info = QString::fromLocal8Bit("【") + "Loading Data Message Info" + QString::fromLocal8Bit("】");
	tBrowser = new QTextBrowser(this);
	tBrowser->setText(info);
	tBrowser->setGeometry(QRect(400, 200, 400, 400));   // X, Y, W, H
	tBrowser->setText(info + "\n- - - - - - - - - - - - - - -\n\n" + "Nothing show");
	//tBrowser->show(); 
	tBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//设置水平滚动条不可见

	searchText = new QLineEdit(this);
	searchText->setObjectName(QStringLiteral("msgLine"));
	searchText->setGeometry(QRect(410, 151, 160, 30));
	QFont font2;
	font2.setPointSize(10);
	searchText->setFont(font2);
	searchText->setText("Input Page No");

	searchBtn = new QPushButton(this);
	searchBtn->setObjectName(QStringLiteral("searchBtn"));
	searchBtn->setGeometry(QRect(590, 151, 70, 30));
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
}

void MainWindow::SavePolyAsFile()
{
}

void MainWindow::SearchByPageNo()
{
	QString value = searchText->text();
	if (value == "" && value != "Input Page No")
	{
		QMessageBox::about(NULL, QString::fromLocal8Bit("Waring"), QString::fromLocal8Bit("SearchAction is not null"));
	}
	else{ 
		QString info = QString::fromLocal8Bit("【") + "Searching Data Message Info" + QString::fromLocal8Bit("】") + "\n- - - - - - - - - - - -\n\n";
		this->tBrowser->setText(info);
		char des[1000];
		bool ok;
		StorageManager.fileOpt.readFile(&dbhead, fileID, des, this, value.toInt(&ok, 10));
	}
}

void MainWindow::InitDB()
{
	//QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromLocal8Bit("InitDB")); 
	strcpy(fileName, "tinydb.mat"); 
	remove(fileName);
	StorageManager.initDB(&dbhead, fileName, this);
	StorageManager.showDbInfo(&dbhead, this);
}

void MainWindow::DeleteFile()
{
	StorageManager.fileOpt.deleteFile(&dbhead, 0, this);
	StorageManager.pageOpt.recyAllPage(&dbhead);
	StorageManager.showDbInfo(&dbhead, this);
}
void MainWindow::CreateFile()
{
	fileID = StorageManager.fileOpt.createFile(&dbhead, NORMAL_FILE, 1, this);
	string s1;
	s1.append("Tiny DB> Message ");
	s1.append(to_string(this->INFO_NUM));
	s1.append(": ");
	s1.append(GetDate());
	s1.append("\n\tCreate File "); 
	s1.append(to_string(fileID));
	s1.append(" Success. \n"); 
	QString str;
	str = this->sysBrowser->toPlainText() + QString::fromStdString(s1);
	this->sysBrowser->setText(str);
	this->INFO_NUM++; 
}
void MainWindow::ReadFile()
{ 
	char des[1000];
	StorageManager.fileOpt.readFile(&dbhead, fileID, des, this);
}
void MainWindow::MemToDisk()
{
	memToDisk(&dbhead);
	StorageManager.showDbInfo(&dbhead, this);
}
void MainWindow::ShowDBInfo()
{
	StorageManager.showDbInfo(&dbhead, this); 
}
void MainWindow::Backup()
{ 
	time_t tt = time(NULL);
	tm* t = localtime(&tt); 
	char time[50];
	sprintf(time, "%d", t->tm_year + 1900);
	char tmp[10];
	sprintf(tmp, "-%d", t->tm_mon + 1);
	strcat(time, tmp);
	sprintf(tmp, "-%d", t->tm_mday);
	strcat(time, tmp);
	sprintf(tmp, " %d", t->tm_min);
	strcat(time, tmp);
	sprintf(tmp, "_%d", t->tm_sec);
	strcat(time, tmp);
	sprintf(tmp, "_%d", t->tm_sec);
	strcat(time, tmp);
	strcat(time, ".mat");
	FILE *fp;
	fp = fopen(fileName, "rb");
	if (fp != NULL){ 
		int b = CopyFile(fileName, time); 
		strcat(time, " Backup Success !");
		QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), b == 1 ? time : QString::fromLocal8Bit("Backup Error !"));
	}
	else{
		QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromLocal8Bit("File is not exist!"));
	}
}

void MainWindow::FreeData()
{
	fclose(dbhead.dataPath);
	free(dbhead.FreeSpace_bitMap); 
}

void MainWindow::LoadData()
{
	std::vector<supplier*> suppliers;
	fileOpt.LoadData(suppliers);
	QString info = QString::fromLocal8Bit("【") + "Loading Data Message Info" + QString::fromLocal8Bit("】") + "\n- - - - - - - - - - - -\n\n";
	this->tBrowser->setText(info);

	for (int i = 0; i < suppliers.size(); i++) { 
		struct supplier *s = suppliers[i];
		char str[1000];
		sprintf(str, "%ld", s->S_superkey);
		strcat(str, s->S_name); 
		strcat(str, s->S_address);
		char tmp[100];
		sprintf(tmp, "%f", s->S_nationkey);
		strcat(str, tmp);
		sprintf(tmp, "%f", s->S_acctbal);
		strcat(str, tmp); 
		strcat(str, s->S_comment);  
		if (i % 100 == 0){
			string s1; 
			s1.append("Tiny DB> ");  
			s1.append(GetDate());
			s1.append("\n\t");
			s1.append(to_string(i) + " - ");
			s1.append(to_string(s->S_superkey));
			s1.append("\n");
			QString info = this->tBrowser->toPlainText() + QString::fromStdString(s1);
			this->tBrowser->setText(info);
		}
		StorageManager.fileOpt.writeFile(&dbhead, fileID, strlen(str), str, this);
	}
	QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromLocal8Bit("Data Loading is completed."));
}

int MainWindow::CopyFile(char *SourceFile, char *NewFile)
{
	ifstream in;
	ofstream out;
	in.open(SourceFile, ios::binary);//打开源文件
	if (in.fail())//打开源文件失败
	{
		cout << "Error 1: Fail to open the source file." << endl;
		in.close();
		out.close();
		return 0;
	}
	out.open(NewFile, ios::binary);//创建目标文件 
	if (out.fail())//创建文件失败
	{
		cout << "Error 2: Fail to create the new file." << endl;
		out.close();
		in.close();
		return 0;
	}
	else//复制文件
	{
		out << in.rdbuf();
		out.close();
		in.close();
		return 1;
	}
}