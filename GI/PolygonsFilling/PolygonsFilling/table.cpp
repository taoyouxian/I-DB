#include "table.h"

#include <QtOpenGL\QtOpenGL>

#include <QtWidgets\QAction>
#include <QtWidgets\QMenu>
#include <QtWidgets\QScrollArea>
#include <QtWidgets\QMenuBar> 
#include <QtWidgets\QApplication>


string dataType[] = { "int", "float", "double", "char", "varchar", "date" };

Table::Table(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle(QString::fromLocal8Bit("Table Creation"));
	connect(ui.cancelBtn, &QPushButton::clicked, this, &Table::SGCancelClick);
	connect(ui.saveBtn, &QPushButton::clicked, this, &Table::SaveTableInfo);
	connect(ui.addBtn, &QPushButton::clicked, this, &Table::AddFiled);
	InitTableWidget();
}

Table::~Table()
{

}

void Table::SaveTableInfo()
{
	QString s;
	int row = ui.tableWidget->rowCount();
	int col = ui.tableWidget->columnCount();
	bool flag = true;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (j != 1){
				if (ui.tableWidget->item(i, 0) == NULL || (ui.tableWidget->item(i, 0) && ui.tableWidget->item(i, 0)->text() == "")){
					flag = false;
					QMessageBox::about(NULL, QString::fromLocal8Bit("Warning"), "Column is null");
					break;
				}
				s = ui.tableWidget->item(i, j)->text() + " ";
			}
			else{
				QString limit = cbos[i]->currentText();
				QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), limit);
			}
		}
		s += "\n";
	}
	if (flag){
		QMessageBox::about(NULL, QString::fromLocal8Bit("Info"), QString::fromLocal8Bit("SaveTable succeed."));
		emit SGSaveClick();
	}
}

// [QTableWidget](http://blog.csdn.net/xgbing/article/details/7774737)
void Table::InitTableWidget()
{
	ui.tableWidget->setRowCount(1);
	ui.tableWidget->setColumnCount(3);

	//自动调整最后一列的宽度使它和表格的右边界对齐  
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);

	//设置表格行标题的对齐方式  
	ui.tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);

	//设置编辑方式  
	ui.tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);

	/*cbo_sex->addItem(QWidget::tr("int"));
	cbo_sex->addItem(QWidget::tr("float"));
	cbo_sex->addItem(QWidget::tr("double"));
	cbo_sex->addItem(QWidget::tr("char"));
	cbo_sex->addItem(QWidget::tr("date"));
	cbo_sex->insertItem(6, tr("Data Type"));
	cbo_sex->insertSeparator(6);*/

	QComboBox *cbo = new QComboBox();
	int size = sizeof(dataType) / sizeof(string);
	for (int i = 0; i < size; i++){
		cbo->addItem(QWidget::tr(dataType[i].c_str()));
	}
	cbos[0] = cbo;
	ui.tableWidget->setCellWidget(0, 1, (QWidget*)cbo);
	connect(cbo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(SelectType(const QString &)));

	// 赋值
	/*QTableWidgetItem *item0 = new QTableWidgetItem;
	item0->setText("");
	ui.tableWidget->setItem(0, 0, item0); */
}

void Table::AddFiled()
{
	int row = ui.tableWidget->rowCount();
	//增加一行  
	ui.tableWidget->setRowCount(++row);

	QComboBox *cbo = new QComboBox();
	int size = sizeof(dataType) / sizeof(string);
	for (int i = 0; i < size; i++){
		cbo->addItem(QWidget::tr(dataType[i].c_str()));
	}
	cbos[row - 1] = cbo;
	ui.tableWidget->setCellWidget(row - 1, 1, (QWidget*)cbo);
	connect(cbo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(SelectType(const QString &)));

	// 赋值
	/*QTableWidgetItem *item0 = new QTableWidgetItem;
	item0->setText("");
	ui.tableWidget->setItem(row - 1, 0, item0);*/
}

void Table::SelectType(const QString &text)
{
	QString str;
	str = "You select " + text;
}