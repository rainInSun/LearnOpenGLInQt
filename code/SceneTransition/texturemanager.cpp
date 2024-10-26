#include "texturemanager.h"
#include <QOpenGLTexture>
#include <QDebug>

TextureManager::TextureManager(QOpenGLContext *pContext)
{
	m_funcs = pContext->versionFunctions<QOpenGLFunctions_3_3_Core>();
	if (!m_funcs)
	{
		qWarning() << "Could not obtain required OpenGL context version";
		Q_ASSERT(false);
	}
}

TextureManager::~TextureManager()
{
}

void TextureManager::initTexture(const QVector<QImage>& imgList)
{
	m_glTextureVec.clear();
	m_glTextureVec.resize(0);
	m_curIndex = 0;
	for (int i = 0; i < imgList.size(); i++)
	{
		QOpenGLTexture *pTexture = new QOpenGLTexture(imgList[i].mirrored());
		std::unique_ptr<QOpenGLTexture> spTexture;
		spTexture.reset(pTexture);
		spTexture->setMinificationFilter(QOpenGLTexture::Linear);
		spTexture->setMagnificationFilter(QOpenGLTexture::Linear);
		m_glTextureVec.push_back(std::move(spTexture));
		/*QImage img = imgList[i].convertToFormat(QImage::Format_RGB888);
		unsigned int texture;
		m_funcs->glGenTextures(1, &texture);
		m_funcs->glBindTexture(GL_TEXTURE_2D, texture);
		m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		m_funcs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		m_funcs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width(), img.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.bits());
		m_funcs->glGenerateMipmap(GL_TEXTURE_2D);
		m_funcs->glBindTexture(GL_TEXTURE_2D, 0);
		m_glTextureVec.push_back(texture);*/
	}
}

void TextureManager::setCurIndex(int curIndex)
{
	m_curIndex = curIndex;
}

int TextureManager::getCurIndex()
{
	return m_curIndex;
}

QOpenGLTexture *TextureManager::getTexture(int index)
{
	if (index < 0 || index >= m_glTextureVec.size())
		return 0;

	return m_glTextureVec[index].get();
}

int TextureManager::getTextureSize()
{
	return m_glTextureVec.size();
}