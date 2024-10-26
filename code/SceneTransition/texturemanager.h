#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QImage>
#include <vector>
#include <memory>

class QOpenGLTexture;

class TextureManager
{
public:
	TextureManager(QOpenGLContext *pContext);
	~TextureManager();

	void initTexture(const QVector<QImage>& imgList);
	void setCurIndex(int curIndex);
	int getCurIndex();
	QOpenGLTexture *getTexture(int index);
	int getTextureSize();

private:
	std::vector<std::unique_ptr<QOpenGLTexture>> m_glTextureVec;
	QOpenGLFunctions_3_3_Core *m_funcs = nullptr;
	int m_curIndex = 0;
};
#endif