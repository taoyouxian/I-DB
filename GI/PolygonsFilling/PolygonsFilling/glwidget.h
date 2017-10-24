#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtOpenGL\QGLWidget>
#include <FilledPolygon.h>


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
	std::vector<FilledPolygon*> m_Polygons;		// ���������
	FilledPolygon *filledPolygon;				// ��ǰ���ڻ��ƵĶ����
	bool	b_StartStatus;						// ��ʼ����һ������εı�־
	bool	b_OnDrawStatus;						// ����״̬
	Point	m_MovePoint;						// �ƶ��ĵ�
	int		windowHeight;						// ����߶� 
	bool    b_FilllStatus;						// �Ƿ����
private:
	void CompleteDraw();						// �Ҽ���ɻ���

public: 

};



#endif // GLWIDGET_H
