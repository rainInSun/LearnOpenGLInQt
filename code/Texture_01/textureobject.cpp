#include "textureobject.h"
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>
#include <QString>
#include <QImage>
#include <QDebug>
// #include <QGLWidget>
#include <QOpenGLTexture>

TextureObject::TextureObject(QOpenGLContext *pContext, const QString& path)
{
    Q_ASSERT(pContext);
    m_funcs = pContext->versionFunctions<QOpenGLFunctions_3_3_Core>();
    if (!m_funcs)
    {
        qWarning() << "Could not obtain required OpenGL context version";
        Q_ASSERT(false);
    }

    // m_funcs->glGenTextures(1, &m_textureID);
    // m_funcs->glBindTexture(GL_TEXTURE_2D, m_textureID);
    // // 为当前绑定的纹理对象设置环绕、过滤方式
    // m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    // m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // QImage img(path);
    // if (img.isNull())
    // {
    //     qWarning() << "Could not find the image";
    //     Q_ASSERT(false);
    // }
    // img = QGLWidget::convertToGLFormat(img);
    // unsigned char *data = img.bits();
    // if (data)
    // {
    //     m_funcs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //     m_funcs->glGenerateMipmap(GL_TEXTURE_2D);
    // }
    // else
    // {
    //     qWarning() << "Failed to load texture";
    //     Q_ASSERT(false);
    // }

    QImage img(path);
    if (img.isNull())
    {
        qWarning() << "Could not find the image";
        Q_ASSERT(false);
    }
    QOpenGLTexture *texture = new QOpenGLTexture(img.mirrored(false, true));
    m_texture.reset(texture);
    m_texture->setMinificationFilter(QOpenGLTexture::Linear);
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    m_texture->setAutoMipMapGenerationEnabled(true);
    m_textureID = m_texture->textureId();
}

TextureObject::~TextureObject()
{
}

unsigned int TextureObject::textureID()
{
    return m_textureID;
}