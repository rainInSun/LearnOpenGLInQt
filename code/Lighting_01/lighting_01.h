#ifndef LIGHTING_01_H
#define LIGHTING_01_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <memory>
#include <QVector3D>
#include <QVector>
#include <QElapsedTimer> 

class ShaderProgram;
class TextureObject;
class Camera;

class Lighting_01 : public QOpenGLWidget
{
    Q_OBJECT

public:
    Lighting_01(QWidget *parent = nullptr);
    ~Lighting_01();

protected:
    void initializeGL() override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    unsigned int m_cubeVAO = 0;
    unsigned int m_lightSourceVAO = 0;
    unsigned int m_VBO = 0;
    bool m_bInit = false;
    QOpenGLFunctions_3_3_Core *m_funcs = nullptr;
    std::unique_ptr<ShaderProgram> m_lightShaderProgram;
    std::unique_ptr<ShaderProgram> m_lightSourceShaderProgram;
    std::unique_ptr<Camera> m_camera;
    bool m_bfirstMouse = true;
    float m_lastMouseX = 0;
    float m_lastMouseY = 0;
    QVector3D m_lightPos = QVector3D(1.2f, 1.0f, 2.0f);
};
#endif