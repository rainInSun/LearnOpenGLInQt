#ifndef TEXTUREOBJECT_H
#define TEXTUREOBJECT_H

class QOpenGLContext;
class QString;
class QOpenGLFunctions_3_3_Core;
class QOpenGLTexture;

#include <memory>

class TextureObject
{
public:
    TextureObject(QOpenGLContext *pContext, const QString& path);
    ~TextureObject();

    unsigned int textureID();

private:
    unsigned int m_textureID = 0;
    QOpenGLFunctions_3_3_Core *m_funcs = nullptr;
    std::unique_ptr<QOpenGLTexture> m_texture;
};


#endif