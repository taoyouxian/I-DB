#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL\QGLWidget> 


class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
	~GLWidget();

protected:  
	// init interface
	void initializeGL();

private:   

public: 

};



#endif // GLWIDGET_H
