//mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets\QMainWindow>
#include <fileOpt.h>
#include "ui_glwidget.h"

#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QLineEdit>
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

private:
	QToolBar	*m_pSavaAsFileToolBar;		// 保存为文件
	QAction		*m_pSavaAsFileAction;		

	QToolBar	*m_pOpenFileToolBar;		// 打开文件
	QAction		*m_pOpenFileAction;	 

	QToolBar	*m_pCleanToolBar;			// 清除面板
	QAction		*m_pCleanAction;	

	QToolBar	*m_pFillToolBar;			// 填充多边形
	QAction		*m_pFillAction;	

	QTextBrowser		*tBrowser;
	QLineEdit		*msg;

/**槽函数**/
public:
	// 文件读取
	void	ReadFiletoPoints();
	// 填充多边形
	void	FillGLWidget();
	// 清除面板
	void	CleanGLWidget();
	// 文件写入
	void	SavePolyAsFile();  
	FileOpt fileOpt;
};

#endif // MAINWINDOW_H