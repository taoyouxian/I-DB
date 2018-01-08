//mainwindow.cpp
#include <QtOpenGL\QtOpenGL>

#include <QtWidgets\QAction>
#include <QtWidgets\QMenu>
#include <QtWidgets\QScrollArea>
#include <QtWidgets\QMenuBar> 
#include <QtWidgets\QApplication>

#include "mainwindow.h"
#include "glwidget.h"  

#include <vector>
#include <list>
using namespace std;

string sqlType[] = { "建立表", "求和(条件)", "等值选择", "范围选择", "投影", "嵌套连接", "排序连接", "散列连接", "三表连接", "插入", "更新", "删除", "丢弃表" };
string compType[] = { ">=", "<=", "<>", "=", "<", ">" };

string sqlSentence[] = {
	"create table employee(rname char(20),ps_partkey int primary key);",
	"select count(*) from partsupp where ps_partkey = 1;",
	"select * from partsupp where ps_partkey = 1;",
	"select * from partsupp where ps_partkey > 1 and ps_partkey < 3;",
	"select ps_partkey,s_superkey,ps_availqty  from partsupp;",
	"select * from supplier, partsupp where supplier.s_superkey = partsupp.s_superkey;",
	"select * from supplier, partsupp where supplier.s_superkey = partsupp.s_superkey;",
	"select * from supplier, partsupp where supplier.s_superkey = partsupp.s_superkey;",
	"select * from supplier, partsupp, nation where supplier.s_superkey = partsupp.s_superkey and partsupp.ps_partkey = nation.ps_partkey;",
	"insert into partsupp(ps_partkey, s_superkey, ps_availqty, ps_supplycost, ps_comment) values(1, 2, 3, 4, 5);",
	"update partsupp set ps_partkey = 3 where ps_partkey = 1;",
	"delete from partsupp where ps_partkey = 1;",
	"drop table partsupp;"
};


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
	//QString info = QString::fromLocal8Bit("【") + "System Message Info" + QString::fromLocal8Bit("】");
	//sysBrowser = new QTextBrowser(this);
	//sysBrowser->setText(info);
	//sysBrowser->setGeometry(QRect(0, 431, 200, 250));   // X, Y, W, H
	//sysBrowser->setText(info + "\n- - - - - - - - - - - -\n" + "No Message.");

	QString info = QString::fromLocal8Bit("【") + "System Tables Info" + QString::fromLocal8Bit("】");
	sysTables = new QTextBrowser(this);
	sysTables->setText(info);
	sysTables->setGeometry(QRect(0, 31, 200, 650));   // X, Y, W, H
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

	cbo = new QComboBox(this);
	cbo->setGeometry(QRect(350, 101, 90, 30));
	int size = sizeof(sqlType) / sizeof(string);
	for (int i = 0; i < size; i++){
		string sType = QString::fromLocal8Bit(sqlType[i].c_str()).toStdString();
		cbo->addItem(QWidget::tr(sType.c_str()));
	}
	connect(cbo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeCboxValue(int)));

	sqlText = new QTextEdit(this);
	sqlText->setGeometry(QRect(200, 131, 760, 230));
	sqlText->setText("No Query.");
	//保持编辑器在光标最后一行  
	QTextCursor cursor = sqlText->textCursor();
	cursor.movePosition(QTextCursor::End);
	sqlText->setTextCursor(cursor);
	//如果超过一定行数清空一下  
	if (sqlText->document()->lineCount() > 20)
	{
		sqlText->clear();
	}

	loadText = new QTextEdit(this);
	loadText->setGeometry(QRect(0, 650, 200, 30));
	loadText->setText(QString::fromLocal8Bit("【TableName】"));
	loadBtn = new QPushButton(this);
	loadBtn->setObjectName(QStringLiteral("loadBtn"));
	loadBtn->setGeometry(QRect(140, 650, 60, 30));
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
	connect(m_QtableWin.get(), &Table::SGCancelClick, this, &MainWindow::BacktoMain);	  // 新建窗口直接返回主窗口
	connect(m_QtableWin.get(), &Table::SGSaveClick, this, &MainWindow::BacktoMain);	// 新建成功后返回

	//关联信号和槽函数  
	connect(m_QtableWin.get(), SIGNAL(SGSaveClick(QString)), this, SLOT(receiveData(QString)));

	m_QtableWin->show();
	//emit SGNewTable();
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

extern "C"
{
	void generate_sql();
}

void MainWindow::QueryBySQL()
{
	/*clock_t startTime, endTime;
	startTime = clock(); */
	DWORD startTime, endTime;
	startTime = GetTickCount();
	QString value = sqlText->toPlainText();

	string sql = value.toStdString();
	ofstream out;
	out.open("in.sql", ios::out);
	if (out.is_open()){
		out << sql;
	}
	out.close();
	generate_sql();
	SQLsenten SQL;//处理成key-value格式
	//SQL.print();

	// 一种符号
	int kind = -1;
	string op = SQL.key[0].substr(0, SQL.key[0].length() - 1);//获取操作符
	if (op == "create"){
		string table_name = SQL.value["table"][0];//表名
		vector<string> column = SQL.value["column"];//列名
		vector<string> data_type = SQL.value["data_type"];//对应每个列的属性
		vector<string> data_length = SQL.value["data_length"];

		if (SQL.value.find("primary_key") != SQL.value.end())
		{
			string primary_key = SQL.value["primary_key"][0];
		}
		int dictID = 0;
		//dictID = createTable(&dbhead, table_name, column, data_type, data_length);
		if (dictID < 0){
			printf("Create table partsupp failed.\n");
			exit(0);
		}
		dbhead.tableName.push_back(table_name);
		dbhead.tableMap[table_name] = dictID;
	}
	else if (op == "update"){
		string table_name = SQL.value["table"][0];//表名
		vector<string> column = SQL.value["column"];
		vector<string> val_list = SQL.value["listnode"];
		vector<string> condition = SQL.value["condition"];

		int table_ID = dbhead.tableMap[table_name];
		//head.data_dict[table_ID].updateRecord(&head,table_name,column,val_list,condition);//更新，需要实现

	}

	else if (op == "insert")
	{
		string table_name = SQL.value["table"][0];//表名
		vector<string> column = SQL.value["column"];//列名
		vector<string> val_list = SQL.value["listnode"];//每列的具体值

		int table_ID = dbhead.tableMap[table_name];//获取表的ID，可以根据表ID获取属性，列名，文件号
		//head.data_dict[table_ID].insertRecord(&head,table_name,column,val_list);//插入，需要实现
		char str[300];
		int recordLen = dbhead.data_dict[partsupp_dictID].recordLength;
		string line;
		for (int i = 0; i < val_list.size(); i++){
			line.append(val_list[i]);
			if (i < val_list.size() - 1)
				line.append("|");
		}
		int min = recordLen - line.length();
		string temp(min, ' ');
		line.append(temp);
		strcpy(str, line.c_str());
		StorageManager.fileOpt.writeFile(&dbhead, table_ID, str, this);
	}
	else if (op == "delete")
	{
		string table_name = SQL.value["table"][0];
		vector<string> condition = SQL.value["condition"];
		//删除
		int table_ID = dbhead.tableMap[table_name];//获取表的ID，可以根据表ID获取属性，列名，文件号
		//head.data_dict[table_ID].deleteRecord(&head,table_name,condition);//删除，需要实现
		int s1 = table_ID;
		vector<std::string> v1;
		string::size_type idx;
		int size = sizeof(compType) / sizeof(string);
		for (int z = 0; z < size; z++){
			idx = condition[0].find(compType[z]);
			//存在
			if (idx != string::npos){
				kind = z;
				break;
			}
		}
		if (condition.size() == 1 && strcmp(condition[0].c_str(), "null") != 0){
			for (int i = 0; i < condition.size(); i++){
				v1 = fileOpt.InfoSplit(condition[i], compType[kind]);
			}
			StorageManager.fileOpt.deleteRecordByCompKey(&dbhead, s1, const_cast<char*>(v1[0].c_str()), const_cast<char*>(compType[kind].c_str()), const_cast<char*>(v1[1].c_str()), this);
		}
		else{
			StorageManager.fileOpt.deleteRecord(&dbhead, s1, this);
		}
	}
	else if (op == "drop"){//删除表；
		string table_name = SQL.value["table"][0];
		vector<string>::iterator it = std::find(dbhead.tableName.begin(), dbhead.tableName.end(), table_name);
		if (it != dbhead.tableName.end()){
			dbhead.tableName.erase(it);
			dbhead.tableMap.erase(table_name);
		}
		StorageManager.showTables(&dbhead, this);
	}
	else if (op == "select")
	{
		vector<string> column = SQL.value["selects"];//列名
		vector<string> table_name = SQL.value["table"];//表名
		vector<string> condition = SQL.value["where_clause"];//条件 （ps_partkey = 1）
		vector<std::string> vv;
		//vector<string> order = SQL.value["order_caluse"];//是否排序 
		if (table_name.size() == 1)
		{
			string table = table_name[0];
			int table_ID = dbhead.tableMap[table];//获取表的ID，可以根据表ID获取属性，列名，文件号
			//head.data_dict[table_ID].selectRecord(&head,column,table,condition);//需要实现
			int s1 = table_ID;
			// condition.size 为0时
			//if (condition.size == 0 && column[0] != "*"){
			//	//s1 = StorageManager.spjOpt.tableScanEqualSelector(&dbhead, table_ID, const_cast<char*>(vv[0].c_str()), const_cast<char*>(vv[1].c_str()));
			//}
			//else
			if (condition.size() == 1 && column[0] != "count(*)" && strcmp(condition[0].c_str(), "null") != 0){
				string::size_type idx;
				int size = sizeof(compType) / sizeof(string);
				for (int z = 0; z < size; z++){
					idx = condition[0].find(compType[z]);
					//存在
					if (idx != string::npos){
						kind = z;
						break;
					}
				}
				for (int i = 0; i < condition.size(); i++){
					vv = fileOpt.InfoSplit(condition[i], compType[kind]);
				}
				s1 = StorageManager.spjOpt.tableScanEqualSelector(&dbhead, table_ID, const_cast<char*>(vv[0].c_str()), const_cast<char*>(compType[kind].c_str()), const_cast<char*>(vv[1].c_str()));
			}
			else if (condition.size() == 2){
				// 2种符号 
				vector<std::string> v1 = fileOpt.InfoSplit(condition[0], ">");
				vector<std::string> v2 = fileOpt.InfoSplit(condition[1], "<");
				s1 = StorageManager.spjOpt.tableScanRangeSelector(&dbhead, table_ID, const_cast<char*>(v1[0].c_str()), const_cast<char*>(v1[1].c_str()), const_cast<char*>(v2[1].c_str()));
			}
			else{
				//s1 = StorageManager.spjOpt.tableScanEqualSelector(&dbhead, table_ID, const_cast<char*>(vv[0].c_str()), const_cast<char*>(vv[1].c_str()));
			}
			string columnStr;
			if (column.size() == 1 && column[0] == "count(*)"){
				columnStr.append(column[0]);
				if (condition.size() == 1 && strcmp(condition[0].c_str(), "null") == 0)
					s1 = StorageManager.spjOpt.tableScanCountSelector(&dbhead, s1);
				else{
					string::size_type idx;
					int size = sizeof(compType) / sizeof(string);
					for (int z = 0; z < size; z++){
						idx = condition[0].find(compType[z]);
						//存在
						if (idx != string::npos){
							kind = z;
							break;
						}
					}
					for (int i = 0; i < condition.size(); i++){
						vv = fileOpt.InfoSplit(condition[i], compType[kind]);
					}
					s1 = StorageManager.spjOpt.tableScanCountSelectorByCond(&dbhead, s1, const_cast<char*>(vv[0].c_str()), const_cast<char*>(compType[kind].c_str()), const_cast<char*>(vv[1].c_str()));
				}
			}
			else if (column.size() == 1 && column[0] == "*"){
				columnStr.append(column[0]);
			}
			else{
				for (int i = 0; i < column.size(); i++){
					columnStr.append(column[i]);
					if (i < column.size() - 1)
						columnStr.append(column[i] + "|");
				}
			}
			StorageManager.fileOpt.readFile(&dbhead, s1, const_cast<char*>(columnStr.c_str()), this);
		}
		else if (table_name.size() == 2){
			//join 大表在前
			string tab1 = table_name[0];
			int tab1ID = dbhead.tableMap[tab1];//获取表的ID，可以根据表ID获取属性，列名，文件号
			string tab2 = table_name[1];
			int tab2ID = dbhead.tableMap[tab2]; 
			// 两表join
			int j2 = -1;
			if (choice == 5)
				j2 = StorageManager.spjOpt.nestedLoopJoin(&dbhead, tab1ID, tab2ID);
			else if (choice == 6)
				j2 = StorageManager.spjOpt.SortJoin(&dbhead, tab1ID, tab2ID);
			else if (choice ==7)
				j2 = StorageManager.spjOpt.HashJoin(&dbhead, tab1ID, tab2ID);
			string columnStr;
			if (column.size() == 1 && column[0] == "count(*)"){
				columnStr.append(column[0]);
			}
			else if (column.size() == 1 && column[0] == "*"){
				columnStr.append(column[0]);
			}
			else{
				for (int i = 0; i < column.size(); i++){
					columnStr.append(column[i]);
					if (i < column.size() - 1)
						columnStr.append(column[i] + "|");
				}
			}
			StorageManager.fileOpt.readFile(&dbhead, j2, const_cast<char*>(columnStr.c_str()), this);
		}
		else if (table_name.size() == 3){
			//join

			string tab1 = table_name[0];
			int tab1ID = dbhead.tableMap[tab1];
			string tab2 = table_name[1];
			int tab2ID = dbhead.tableMap[tab2];
			string tab3 = table_name[2];
			int tab3ID = dbhead.tableMap[tab3];
			int j2 = StorageManager.spjOpt.nestedLoopJoin(&dbhead, tab1ID, tab2ID);
			int j3 = StorageManager.spjOpt.nestedLoopJoinByThree(&dbhead, j2, tab2ID, tab3ID);
			string columnStr;
			if (column.size() == 1 && column[0] == "count(*)"){
				columnStr.append(column[0]);
			}
			else if (column.size() == 1 && column[0] == "*"){
				columnStr.append(column[0]);
			}
			else{
				for (int i = 0; i < column.size(); i++){
					columnStr.append(column[i]);
					if (i < column.size() - 1)
						columnStr.append(column[i] + "|");
				}
			}
			StorageManager.fileOpt.readFile(&dbhead, j2, const_cast<char*>(columnStr.c_str()), this);
		}
	}
	endTime = GetTickCount();
	std::stringstream ss;
	double time = (double)(endTime - startTime);
	ss << time;
	QString info = QString::fromLocal8Bit("【") + "Result" + QString::fromLocal8Bit("】");
	tQuery->setText(info + "\n" + QString::fromStdString(op) + " time: " + QString::fromStdString(ss.str()) + "ms");
	//QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromStdString(ss.str()));
}

void MainWindow::LoadByTableName()
{
	DWORD startTime, endTime;
	QString value = loadText->toPlainText();
	int fileID = dbhead.tableMap[value.toStdString()];

	int recordLen = dbhead.data_dict[fileID].recordLength;
	QString fileName = fileOpt.OpenFileDialog();
	startTime = GetTickCount();
	if (fileName.isNull()) {
		;
	}
	else {
		char str[3000];
		string line;
		ifstream fin(fileName.toStdString());
		int num = 0;
		sqlText->setText(QString::fromLocal8Bit("进度条") + ": | - - 0%|");
		string s1 = "进度条: | - - ";
		int process;
		while (!fin.eof()) {
			if (num % 30 == 0){
				process = 1.0 * num / 30;
				s1.append("- - - ");
			}
			num++;
			getline(fin, line);
			int min = recordLen - line.length();
			string temp(min, ' ');
			line.append(temp);
			strcpy(str, line.c_str());
			StorageManager.fileOpt.writeFile(&dbhead, fileID, str, this);
			sqlText->setText(QString::fromStdString(s1 + to_string(process) + "%|"));
		}
		sqlText->setText(QString::fromLocal8Bit("进度条") + ": |- - - - - - - - - - - - - - - - - -- - - - -100%|");
		fin.close();
	}
	endTime = GetTickCount();
	std::stringstream ss;
	double time = (double)(endTime - startTime);
	ss << time;
	QString info = QString::fromLocal8Bit("【") + "Result" + QString::fromLocal8Bit("】");
	tQuery->setText(info + "\n Loading Data Time: " + QString::fromStdString(ss.str()) + "ms");
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
	StorageManager.showTables(&dbhead, this);
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
	this->fileLog = "";
	StorageManager.show_SysDesc(&dbhead, this);
}
void MainWindow::Backup()
{

}

void MainWindow::ShowFileDesc()
{
	this->fileLog = "";
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

void MainWindow::changeCboxValue(int index)
{
	string sql = sqlSentence[index];
	choice = index;
	sqlText->setText(QString::fromStdString(sql));
}

void MainWindow::receiveData(QString data)
{
	string value = data.toStdString();  //获取传递过来的数据 
}