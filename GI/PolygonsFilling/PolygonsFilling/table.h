#ifndef TABLE_H
#define TABLE_H

#include <QWidget>
#include "ui_table.h"
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


class Table : public QWidget
{
	Q_OBJECT

public:
	Table(QWidget *parent = 0);
	~Table();
	QComboBox *cbos[15];

private:
	Ui::table ui;
	void BacktoMain() { emit SGCancelClick(); }
	void AddFiled();
	void InitTableWidget();
	void SelectType(const QString &text);

private slots:
	void SaveTableInfo();


signals:
	void SGCancelClick();
	void SGSaveClick(QString data);    //在子窗口创建public函数用来获取传递的数据  

};

#endif // TABLE_H
