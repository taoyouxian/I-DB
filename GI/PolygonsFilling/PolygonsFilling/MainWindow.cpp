//mainwindow.cpp
#include <QtOpenGL\QtOpenGL>

#include <QtWidgets\QAction>
#include <QtWidgets\QMenu>
#include <QtWidgets\QScrollArea>
#include <QtWidgets\QMenuBar> 
#include <QtWidgets\QApplication>

#include "mainwindow.h"
#include "glwidget.h"
#include "table.h"

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
	InitDB();
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

	setWindowTitle(QString::fromLocal8Bit("I Love Database【I*DB】 —— Principle and Realization of Database Management System"));
	resize(960, 680);
}

void MainWindow::CreateToolBar()
{

	m_pNewAction = new QAction(QString::fromLocal8Bit("New"), this);
	connect(m_pNewAction, &QAction::triggered, this, &MainWindow::NewTable);
	m_pNewToolBar = addToolBar(tr("New"));
	m_pNewToolBar->addAction(m_pNewAction);
	m_pNewToolBar->setAttribute(Qt::WA_DeleteOnClose);

	m_pQueryAction = new QAction(QString::fromLocal8Bit("Query"), this);
	connect(m_pQueryAction, &QAction::triggered, this, &MainWindow::Query);
	m_pQueryToolBar = addToolBar(tr("Query"));
	m_pQueryToolBar->addAction(m_pQueryAction);
	m_pQueryToolBar->setAttribute(Qt::WA_DeleteOnClose);

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

	/*m_pLoadDataAction = new QAction(QString::fromLocal8Bit("LoadData"), this);
	connect(m_pLoadDataAction, &QAction::triggered, this, &MainWindow::LoadData);
	m_pLoadDataToolBar = addToolBar(tr("LoadData"));
	m_pLoadDataToolBar->addAction(m_pLoadDataAction);
	m_pLoadDataToolBar->setAttribute(Qt::WA_DeleteOnClose);*/

	/*m_pReadFileAction = new QAction(QString::fromLocal8Bit("ReadFile"), this);
	connect(m_pReadFileAction, &QAction::triggered, this, &MainWindow::ReadFile);
	m_pReadFileToolBar = addToolBar(tr("ReadFile"));
	m_pReadFileToolBar->addAction(m_pReadFileAction);
	m_pReadFileToolBar->setAttribute(Qt::WA_DeleteOnClose);*/

	m_pShowDBInfoAction = new QAction(QString::fromLocal8Bit("ShowDBInfo"), this);
	connect(m_pShowDBInfoAction, &QAction::triggered, this, &MainWindow::ShowDBInfo);
	m_pShowDBInfoToolBar = addToolBar(tr("ShowDBInfo"));
	m_pShowDBInfoToolBar->addAction(m_pShowDBInfoAction);
	m_pShowDBInfoToolBar->setAttribute(Qt::WA_DeleteOnClose); 

	m_pFileDescAction = new QAction(QString::fromLocal8Bit("ShowFileDesc"), this);
	connect(m_pFileDescAction, &QAction::triggered, this, &MainWindow::ShowFileDesc);
	m_pFileDescToolBar = addToolBar(tr("ShowFileDesc"));
	m_pFileDescToolBar->addAction(m_pFileDescAction);
	m_pFileDescToolBar->setAttribute(Qt::WA_DeleteOnClose);

	m_pBackupAction = new QAction(QString::fromLocal8Bit("Backup"), this);
	connect(m_pBackupAction, &QAction::triggered, this, &MainWindow::Backup);
	m_pBackupToolBar = addToolBar(tr("Backup"));
	m_pBackupToolBar->addAction(m_pBackupAction);
	m_pBackupToolBar->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::CreateView() {
	QString info = QString::fromLocal8Bit("【") + "System Message Info" + QString::fromLocal8Bit("】");
	sysBrowser = new QTextBrowser(this);
	sysBrowser->setText(info);
	sysBrowser->setGeometry(QRect(0, 431, 200, 250));   // X, Y, W, H
	sysBrowser->setText(info + "\n- - - - - - - - - - - -\n" + "No Message.");

	info = QString::fromLocal8Bit("【") + "System Tables Info" + QString::fromLocal8Bit("】");
	sysTables = new QTextBrowser(this);
	sysTables->setText(info);
	sysTables->setGeometry(QRect(0, 31, 200, 400));   // X, Y, W, H
	sysTables->setText(info + "\n- - - - - - - - - - - -\n" + "No Tables.");

	info = QString::fromLocal8Bit("【") + "Demonstration Steps" + QString::fromLocal8Bit("】");
	showBrowser = new QTextBrowser(this);
	showBrowser->setText(info);
	showBrowser->setGeometry(QRect(200, 31, 760, 70));   // X, Y, W, H
	showBrowser->setText(info + "\n- - - - - - - - - - - - - - -\n" + "initDB -> deleteFile -> createFile -> memToDisk -> writeFile(loadData) -> readFile -> memToDisk, ShowFileDesc(showDbInfo)");

	info = QString::fromLocal8Bit("【") + "Query SQL Panel" + QString::fromLocal8Bit("】");
	tBrowser = new QTextBrowser(this);
	tBrowser->setText(info);
	tBrowser->setGeometry(QRect(200, 101, 760, 230));   // X, Y, W, H
	//tBrowser->setText(info + "\n- - - - - - - - - - - - - - -\n" + "No Query.");
	//tBrowser->show(); 
	tBrowser->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//设置水平滚动条不可见

	sqlText = new QTextEdit(this);
	sqlText->setGeometry(QRect(200, 131, 760, 230));
	sqlText->setText("No Query.");
	//保持编辑器在光标最后一行  
	QTextCursor cursor = sqlText->textCursor();
	cursor.movePosition(QTextCursor::End);
	sqlText->setTextCursor(cursor);
	//如果超过一定行数清空一下  
	if (sqlText->document()->lineCount()>20)
	{ 
		sqlText->clear();
	}

	loadText = new QTextEdit(this);
	loadText->setGeometry(QRect(0, 400, 200, 30));
	loadText->setText("TableName");
	loadBtn = new QPushButton(this);
	loadBtn->setObjectName(QStringLiteral("loadBtn"));
	loadBtn->setGeometry(QRect(140, 400, 60, 30));
	loadBtn->setText("LoadData");
	loadBtn->setStyleSheet("QPushButton{background-color:white; color: black; border - radius: 10px;  border: 1px groove gray; border - style: outset;}" "QPushButton:hover{background-color:rgb(85, 170, 255); color: white;}" "QPushButton:pressed{background-color:rgb(85, 85, 255); border - style: inset; }");
	connect(loadBtn, &QPushButton::clicked, this, &MainWindow::LoadByTableName);


	queryBtn = new QPushButton(this);
	queryBtn->setObjectName(QStringLiteral("queryBtn"));
	queryBtn->setGeometry(QRect(900, 101, 60, 30));
	queryBtn->setText("Query");
	queryBtn->setStyleSheet("QPushButton{background-color:white; color: black; border - radius: 10px;  border: 1px groove gray; border - style: outset;}" "QPushButton:hover{background-color:rgb(85, 170, 255); color: white;}" "QPushButton:pressed{background-color:rgb(85, 85, 255); border - style: inset; }");
	connect(queryBtn, &QPushButton::clicked, this, &MainWindow::QueryBySQL);

	tableWidget = new QTableWidget(this);  //新建一4行2列的表格  
	/*tableWidget->setRowCount(8);
	tableWidget->setColumnCount(7);*/

	////设置表格行标题  
	//QStringList headerLabels;
	//headerLabels << "C1" << "C2" << "C3" << "Ck" << "..." << "Cn-1" << "Cn";
	tableWidget->setGeometry(QRect(200, 360, 760, 230));   // X, Y, W, H
	tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	//tableWidget->setHorizontalHeaderLabels(headerLabels); 

	////设置表格行标题的对齐方式  
	//tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

	////设置行表题  
	//QStringList rowLabels;
	//rowLabels << "L1" << "L2" << "L3" << "L4" << "Lk" << "..." << "Ln-1" << "Ln";
	//tableWidget->setVerticalHeaderLabels(rowLabels);
	// 
	////自动调整最后一列的宽度使它和表格的右边界对齐  
	//tableWidget->horizontalHeader()->setStretchLastSection(true);

	////设置表格的选择方式  
	//tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems); 

	info = QString::fromLocal8Bit("【") + "Result" + QString::fromLocal8Bit("】");
	tQuery = new QTextBrowser(this);
	tQuery->setText(info);
	tQuery->setGeometry(QRect(200, 590, 760, 90));   // X, Y, W, H 
	tQuery->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);//设置水平滚动条不可见
}

void MainWindow::NewTable() {
	if (m_QtableWin.get() == nullptr) {
		m_QtableWin = make_shared<Table>();
	}
	m_QtableWin->show();
	connect(m_QtableWin.get(), &Table::SGCancelClick, this, &MainWindow::BacktoMain);	  // 新建窗口直接返回主窗口
	connect(m_QtableWin.get(), &Table::SGSaveClick, this, &MainWindow::BacktoMain);	// 新建成功后返回
	emit SGNewTable();
}

//返回主窗口
void MainWindow::BacktoMain()
{
	if (m_QtableWin.get() != nullptr)
		m_QtableWin->close();
	// 其他操作
}

void MainWindow::Query() {

}

void MainWindow::ReadFiletoPoints() {
}

void MainWindow::SavePolyAsFile()
{
}

void MainWindow::QueryBySQL()
{
	/*clock_t startTime, endTime;
	startTime = clock(); */
	DWORD startTime, endTime;
	startTime = GetTickCount();
	QString value = sqlText->toPlainText();
	string tablname = value.toStdString();
	int s = dbhead.tableMap[tablname];
	char* attribute_name;
	if (strcmp("supplier", tablname.c_str()) == 0){
		attribute_name = "*";
		//int s1 = StorageManager.spjOpt.tableScanEqualSelector(&dbhead, supplier_dictID, "S_superkey", "56");
		//int s1 = StorageManager.spjOpt.tableScanEqualSelector(&dbhead, s, "S_superkey", "56");
		//endTime = clock();
		endTime = GetTickCount();
		StorageManager.fileOpt.readFile(&dbhead, supplier_dictID, attribute_name, this);

		//attribute_name = "S_superkey|S_name";
		//int p1 = StorageManager.spjOpt.projection(&dbhead, supplier_dictID, attribute_name);
		//int p1 = StorageManager.spjOpt.projection(&dbhead, s, attribute_name);
		//StorageManager.fileOpt.readFile(&dbhead, p1, attribute_name, this);
	} else{
		attribute_name = "count(*)";
		int s1 = StorageManager.spjOpt.tableScanCountSelector(&dbhead, s);
		//endTime = clock();
		endTime = GetTickCount();
		StorageManager.fileOpt.readFile(&dbhead, s1, attribute_name, this);
	}
	std::stringstream ss;
	double time = (double)(endTime - startTime);
	ss << time; 
	QString info = QString::fromLocal8Bit("【") + "Result" + QString::fromLocal8Bit("】");
	tQuery->setText(info + "\n Time Cost: " + QString::fromStdString(ss.str()) + "ms");
	//QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromStdString(ss.str()));
}

void MainWindow::LoadByTableName()
{  
	QString value = loadText->toPlainText();
	int fileID = dbhead.tableMap[value.toStdString()];

	QString fileName = fileOpt.OpenFileDialog();
	if (fileName.isNull()) {
		;
	}
	else {
		char str[3000];
		string line;
		ifstream fin(fileName.toStdString());
		int num = 0;
		sqlText->setText("| - - 0%|");
		string s1 = "| - - ";
		int process;
		while (!fin.eof()) {
			if (num % 30 == 0){
				process = 1.0 * num / 30;
				s1.append("- - "); 
			}
			num++;
			getline(fin, line);
			int min = 230 - line.length();
			string temp(min, ' ');
			line.append(temp);
			strcpy(str, line.c_str());
			StorageManager.fileOpt.writeFile(&dbhead, fileID, str, this);
			sqlText->setText(QString::fromStdString(s1 + to_string(process) + "%|"));
		}
		sqlText->setText("|- - - - - - - - - - - - - - -- - - - -100%|");
		fin.close();
	}
}

void MainWindow::SearchByPageNo()
{
	 
}

void MainWindow::InitDB()
{
	StorageManager.welcome(this);
	char filename[100];
	strcpy(filename, PATH);
	strcat(filename, "database.mat\0");
	remove("database.mat");  
	StorageManager.init_database(&dbhead, filename, this);
	StorageManager.show_SysDesc(&dbhead, this);
	StorageManager.init_MapData(&dbhead, this);
	supplier_dictID = StorageManager.init_Table(&dbhead, this);

	string s1;
	QString info = QString::fromLocal8Bit("【") + "System Tables Info" + QString::fromLocal8Bit("】");
	for (int i = 0; i < dbhead.tableName.size(); i++){
		string tablename = dbhead.tableName[i];
		int s = dbhead.tableMap[tablename];
		string str = to_string(s);
		s1.append(str + "  " + tablename + "\n");
	}
	sysTables->setText(info + "\n- - - - - - - - - - - -\n" + QString::fromStdString(s1));
}

void MainWindow::DeleteFile()
{
	 
}
void MainWindow::CreateFile()
{
	 
}
void MainWindow::ReadFile()
{ 
	 
}
void MainWindow::MemToDisk()
{
	 
}
void MainWindow::ShowDBInfo()
{

}
void MainWindow::Backup()
{ 
	 
}

void MainWindow::ShowFileDesc()
{ 
	for (int i = 0; i < dbhead.tableName.size(); i++){
		string tablename = dbhead.tableName[i];
		int s = dbhead.tableMap[tablename];
		StorageManager.show_FileDesc(&dbhead, s, this);
	}
}

void MainWindow::LoadData()
{
	 
}

int MainWindow::CopyFile(char *SourceFile, char *NewFile)
{
	 
	return 1;
}