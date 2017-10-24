/********************************************************************************
** Form generated from reading UI file 'tinydbms.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TINYDBMS_H
#define UI_TINYDBMS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_tinydbmsClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *tinydbmsClass)
    {
        if (tinydbmsClass->objectName().isEmpty())
            tinydbmsClass->setObjectName(QStringLiteral("tinydbmsClass"));
        tinydbmsClass->resize(600, 400);
        menuBar = new QMenuBar(tinydbmsClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        tinydbmsClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(tinydbmsClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        tinydbmsClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(tinydbmsClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tinydbmsClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(tinydbmsClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        tinydbmsClass->setStatusBar(statusBar);

        retranslateUi(tinydbmsClass);

        QMetaObject::connectSlotsByName(tinydbmsClass);
    } // setupUi

    void retranslateUi(QMainWindow *tinydbmsClass)
    {
        tinydbmsClass->setWindowTitle(QApplication::translate("tinydbmsClass", "tinydbms", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class tinydbmsClass: public Ui_tinydbmsClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TINYDBMS_H
