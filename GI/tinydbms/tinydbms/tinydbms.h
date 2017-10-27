#ifndef TINYDBMS_H
#define TINYDBMS_H

#include <QtWidgets/QMainWindow>
#include "ui_tinydbms.h"

class tinydbms : public QMainWindow
{
	Q_OBJECT

public:
	tinydbms(QWidget *parent = 0);
	~tinydbms();

private:
	Ui::tinydbmsClass ui;
};

#endif // TINYDBMS_H
