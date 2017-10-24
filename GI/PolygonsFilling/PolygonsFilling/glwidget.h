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
	std::vector<FilledPolygon*> m_Polygons;		// 多边形序列
	FilledPolygon *filledPolygon;				// 当前正在绘制的多边形
	bool	b_StartStatus;						// 开始绘制一个多边形的标志
	bool	b_OnDrawStatus;						// 绘制状态
	Point	m_MovePoint;						// 移动的点
	int		windowHeight;						// 窗体高度 
	bool    b_FilllStatus;						// 是否填充
private:
	void CompleteDraw();						// 右键完成绘制

public: 

};



#endif // GLWIDGET_H
