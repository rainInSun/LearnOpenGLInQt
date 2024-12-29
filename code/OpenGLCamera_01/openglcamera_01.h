#ifndef OPENGL_CAMERA_01_H
#define OPENGL_CAMERA_01_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <memory>
#include <QVector3D>
#include <QVector>
#include <QElapsedTimer> 

class ShaderProgram;
class TextureObject;
class Camera;

class OpenGLCamera_01 : public QOpenGLWidget
{
    Q_OBJECT

public:
    OpenGLCamera_01(QWidget *parent = nullptr);
    ~OpenGLCamera_01();

protected:
    void initializeGL() override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    bool m_bInit = false;
    QOpenGLFunctions_3_3_Core *m_funcs = nullptr;
    std::unique_ptr<ShaderProgram> m_shaderProgram;
    std::unique_ptr<TextureObject> m_texture0;
    std::unique_ptr<TextureObject> m_texture1;
    std::unique_ptr<Camera> m_camera;
    QVector<QVector3D> m_cubePositions;
    bool m_bfirstMouse = true;
    float m_lastMouseX = 0;
    float m_lastMouseY = 0;
};
#endif