#ifndef COORDINATE_SYSTEM_01_H
#define COORDINATE_SYSTEM_01_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <memory>
#include <QVector3D>
#include <QVector>

class ShaderProgram;
class TextureObject;

class CoordinateSystem_01 : public QOpenGLWidget
{
    Q_OBJECT

public:
    CoordinateSystem_01(QWidget *parent = nullptr);
    ~CoordinateSystem_01();

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    bool m_bInit = false;
    QOpenGLFunctions_3_3_Core *m_funcs = nullptr;
    std::unique_ptr<ShaderProgram> m_shaderProgram;
    std::unique_ptr<TextureObject> m_texture0;
    std::unique_ptr<TextureObject> m_texture1;
    QVector<QVector3D> m_cubePositions;
};
#endif