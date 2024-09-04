#ifndef HELLOTRIANGLE_01_H
#define HELLOTRIANGLE_01_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class GLSL_02 : public QOpenGLWidget
{
	Q_OBJECT

public:
	GLSL_02(QWidget *parent = nullptr);
	~GLSL_02();

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_program = 0;
    bool m_bInit = false;
    QOpenGLFunctions_3_3_Core *m_funcs = nullptr;
};
#endif