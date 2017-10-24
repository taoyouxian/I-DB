
//glwidget.cpp
#include <QtGui\QtGui>
#include <QMouseEvent>
#include "glwidget.h"


GLWidget::GLWidget(QWidget *parent) :
QGLWidget(parent)
{
	b_StartStatus = true;
	b_OnDrawStatus = false;
	b_FilllStatus = false;
	m_MovePoint = Point(-1, -1);
	filledPolygon = nullptr;
	this->setMouseTracking(true);   // Êó±ê×·×Ù
}

GLWidget::~GLWidget() {

}  

void GLWidget::initializeGL()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnableClientState(GL_VERTEX_ARRAY);
}