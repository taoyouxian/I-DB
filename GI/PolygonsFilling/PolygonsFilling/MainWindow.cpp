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

	setWindowTitle(QString::fromLocal8Bit("Tiny Database ¡ª¡ª Principle and Realization of Database Management System"));
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

	m_pFillAction = new QAction(QString::fromLocal8Bit("Create"), this);
	connect(m_pFillAction, &QAction::triggered, this, &MainWindow::FillGLWidget);
	m_pFillToolBar = addToolBar(tr("create"));
	m_pFillToolBar->addAction(m_pFillAction);
	m_pFillToolBar->setAttribute(Qt::WA_DeleteOnClose);

	m_pCleanAction = new QAction(QString::fromLocal8Bit("Delete"), this);
	connect(m_pCleanAction, &QAction::triggered, this, &MainWindow::CleanGLWidget);
	m_pCleanToolBar = addToolBar(tr("delete"));
	m_pCleanToolBar->addAction(m_pCleanAction);
	m_pCleanToolBar->setAttribute(Qt::WA_DeleteOnClose);

}

void MainWindow::CreateView() {
	QString info = "Message Info:"; 
	tBrowser = new QTextBrowser(this);
	tBrowser->setText(info);
	tBrowser->setGeometry(QRect(0, 300, 800, 300));   // X, Y, W, H
	tBrowser->setText(info + "\n" + "Nothing show");
	tBrowser->show(); 

	msg = new QLineEdit(this);
	msg->setObjectName(QStringLiteral("msg"));
	msg->setGeometry(QRect(0, 100, 200, 30));
	QFont font2;
	font2.setPointSize(14);
	msg->setFont(font2); 
}  

//void MainWindow::ShowInfo() {
//
//}

void MainWindow::FillGLWidget() { 

} 

void MainWindow::CleanGLWidget() {

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