/********************************************************************************
** Form generated from reading UI file 'glwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GLWIDGET_H
#define UI_GLWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GLWidgetClass
{
public:

    void setupUi(QWidget *GLWidgetClass)
    {
        if (GLWidgetClass->objectName().isEmpty())
            GLWidgetClass->setObjectName(QStringLiteral("GLWidgetClass"));
        GLWidgetClass->resize(600, 400);

        retranslateUi(GLWidgetClass);

        QMetaObject::connectSlotsByName(GLWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *GLWidgetClass)
    {
        GLWidgetClass->setWindowTitle(QApplication::translate("GLWidgetClass", "GLWidget", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class GLWidgetClass: public Ui_GLWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GLWIDGET_H
