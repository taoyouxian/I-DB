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
	
	// ����������
	void	CreateToolBar();
	// ��������
	void	CreateView();
	// ��ʼ����ʾ��
	void	InitViewWidget();

private:
	QToolBar	*m_pSavaAsFileToolBar;		// ����Ϊ�ļ�
	QAction		*m_pSavaAsFileAction;		

	QToolBar	*m_pOpenFileToolBar;		// ���ļ�
	QAction		*m_pOpenFileAction;	 

	QToolBar	*m_pCleanToolBar;			// ������
	QAction		*m_pCleanAction;	

	QToolBar	*m_pFillToolBar;			// �������
	QAction		*m_pFillAction;	

	QTextBrowser		*tBrowser;
	QLineEdit		*msg;

/**�ۺ���**/
public:
	// �ļ���ȡ
	void	ReadFiletoPoints();
	// �������
	void	FillGLWidget();
	// ������
	void	CleanGLWidget();
	// �ļ�д��
	void	SavePolyAsFile();  
	FileOpt fileOpt;
};

#endif // MAINWINDOW_H