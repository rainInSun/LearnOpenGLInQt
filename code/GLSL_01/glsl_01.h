#ifndef HELLOTRIANGLE_01_H
#define HELLOTRIANGLE_01_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QElapsedTimer>
#include <QTimer>
#include <memory>

class Shader;

class GLSL_01 : public QOpenGLWidget
{
	Q_OBJECT

public:
	GLSL_01(QWidget *parent = nullptr);
	~GLSL_01();

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    //unsigned int m_program = 0;
    bool m_bInit = false;
    QOpenGLFunctions_3_3_Core *m_funcs = nullptr;
    QElapsedTimer m_elapsedTimer;
    QTimer m_timer;
    
    std::unique_ptr<Shader> m_shader;
};
#endif