#ifndef TEXTURE_01_H
#define TEXTURE_01_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <memory>

class ShaderProgram;
class TextureObject;

class Texture_01 : public QOpenGLWidget
{
	Q_OBJECT

public:
	Texture_01(QWidget *parent = nullptr);
	~Texture_01();

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    unsigned int m_VAO = 0;
    unsigned int m_VBO = 0;
    unsigned int m_EBO = 0;
    bool m_bInit = false;
    QOpenGLFunctions_3_3_Core *m_funcs = nullptr;
    std::unique_ptr<ShaderProgram> m_shaderProgram;
    std::unique_ptr<TextureObject> m_texture0;
    std::unique_ptr<TextureObject> m_texture1;
};
#endif