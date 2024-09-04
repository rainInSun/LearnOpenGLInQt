#ifndef HELLOTRIANGLE_01_H
#define HELLOTRIANGLE_01_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class HelloTriangle_01 : public QOpenGLWidget
{
	Q_OBJECT

public:
	HelloTriangle_01(QWidget *parent = nullptr);
	~HelloTriangle_01();

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;
    unsigned int m_program = 0;
    bool m_bInit = false;
    QOpenGLFunctions_3_3_Core *m_funcs = nullptr;

};
#endif