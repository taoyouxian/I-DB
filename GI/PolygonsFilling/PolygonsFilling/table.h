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
	 
signals:
	void SGCancelClick();
	void SGSaveClick();

private:
	Ui::table ui;
	void BacktoMain() { emit SGCancelClick(); }
	void SaveTableInfo();
	void AddFiled();
	void InitTableWidget();
	void SelectType(const QString &text);


private:

public:
	

};

#endif // TABLE_H
