#include "scenetransition.h"
#include <QOpenGLContext>
#include <QOpenGLTexture>
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include "texturemanager.h"
#include "shaderprogrammanager.h"

class SceneTransitionImpl
{
public:
	SceneTransitionImpl() {}
	~SceneTransitionImpl() {}

public:
	bool m_bInit = false;
	TransitionType m_TransitionType = TransitionType::Fade;
	TransitionAction m_action = TransitionAction::None;
	int m_duration = 1000; // ms
	QVector<QImage> m_imgList;
	QOpenGLFunctions_3_3_Core *m_funcs = nullptr;
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
	unsigned int m_EBO = 0;
	QTimer m_timer;
	QElapsedTimer m_elapsedTimer;
	float m_ratio = 1.0f;

	std::unique_ptr<TextureManager> m_textureManager;
	std::unique_ptr<ShaderProgramManager> m_programManager;
};

SceneTransition::SceneTransition(QWidget *parent)
	: QOpenGLWidget(parent)
{
	SceneTransitionImpl *pImpl = new SceneTransitionImpl();
	m_impl.reset(pImpl);
}

SceneTransition::~SceneTransition()
{
    if (m_impl->m_bInit)
    {
        m_impl->m_funcs->glDeleteVertexArrays(1, &m_impl->m_VAO);
        m_impl->m_funcs->glDeleteBuffers(1, &m_impl->m_VBO);
		m_impl->m_funcs->glDeleteBuffers(1, &m_impl->m_EBO);
    }
}

void SceneTransition::goNext()
{
	if (m_impl->m_timer.isActive())
		return;

	if (m_impl->m_textureManager->getCurIndex() + 1 >= m_impl->m_textureManager->getTextureSize())
		return;

	QOpenGLTexture *pTexture = m_impl->m_textureManager->getTexture(m_impl->m_textureManager->getCurIndex());
	QOpenGLTexture *pNextTexture = m_impl->m_textureManager->getTexture(m_impl->m_textureManager->getCurIndex() + 1);
	if (!pTexture || !pNextTexture)
		return;

	makeCurrent();
	m_impl->m_programManager->setFloatUniform(m_impl->m_TransitionType, "progress", 0.0f);
	m_impl->m_funcs->glActiveTexture(GL_TEXTURE0);
	m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, pTexture->textureId());
	m_impl->m_funcs->glActiveTexture(GL_TEXTURE1);
	m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, pNextTexture->textureId());
	m_impl->m_timer.start(10);
	m_impl->m_elapsedTimer.restart();
	m_impl->m_action = TransitionAction::Next;
	doneCurrent();
}

void SceneTransition::goPrev()
{
	if (m_impl->m_timer.isActive())
		return;

	if (m_impl->m_textureManager->getCurIndex() - 1 < 0)
		return;

	QOpenGLTexture *pTexture = m_impl->m_textureManager->getTexture(m_impl->m_textureManager->getCurIndex());
	QOpenGLTexture *pPrevTexture = m_impl->m_textureManager->getTexture(m_impl->m_textureManager->getCurIndex() - 1);
	if (!pTexture || !pPrevTexture)
		return;

	makeCurrent();
	m_impl->m_programManager->setFloatUniform(m_impl->m_TransitionType, "progress", 1.0f);
	m_impl->m_funcs->glActiveTexture(GL_TEXTURE0);
	m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, pPrevTexture->textureId());
	m_impl->m_funcs->glActiveTexture(GL_TEXTURE1);
	m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, pTexture->textureId());
	m_impl->m_timer.start(10);
	m_impl->m_elapsedTimer.restart();
	m_impl->m_action = TransitionAction::Prev;
}

void SceneTransition::setTransitionType(TransitionType type)
{
	m_impl->m_TransitionType = type;
	if (m_impl->m_bInit)
	{
		makeCurrent();
		m_impl->m_programManager->setIntUniform(m_impl->m_TransitionType, "texture0", 0);
		m_impl->m_programManager->setIntUniform(m_impl->m_TransitionType, "texture1", 1);
	}
}

void SceneTransition::setTransitionDuration(int duration)
{
	if (duration <= 0)
		return;

	m_impl->m_duration = duration;
}

void SceneTransition::setImgList(const QVector<QImage>& imgList)
{
	m_impl->m_imgList = imgList;
	if (m_impl->m_bInit)
	{
		makeCurrent();
		m_impl->m_funcs->glActiveTexture(GL_TEXTURE0);
		m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, 0);
		m_impl->m_funcs->glActiveTexture(GL_TEXTURE1);
		m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, 0);

		m_impl->m_textureManager->initTexture(m_impl->m_imgList);
		m_impl->m_imgList.clear();
		QOpenGLTexture *pTexture = m_impl->m_textureManager->getTexture(m_impl->m_textureManager->getCurIndex());
		if (pTexture)
		{
			m_impl->m_funcs->glActiveTexture(GL_TEXTURE0);
			m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, pTexture->textureId());
			m_impl->m_funcs->glActiveTexture(GL_TEXTURE1);
			m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, pTexture->textureId());
		}
		update();
	}
}

void SceneTransition::initializeGL()
{
	if (m_impl->m_bInit)
		return;

	m_impl->m_bInit = true;
	m_impl->m_action = TransitionAction::None;
	makeCurrent();
	QOpenGLContext *pContext = context();
	if (pContext)
		m_impl->m_funcs = pContext->versionFunctions<QOpenGLFunctions_3_3_Core>();
	if (!m_impl->m_funcs)
	{
		qWarning() << "Could not obtain required OpenGL context version";
		Q_ASSERT(false);
		return;
	}

	m_impl->m_funcs->glViewport(0, 0, width(), height());
	TextureManager *pTextureMgr = new TextureManager(pContext);
	m_impl->m_textureManager.reset(pTextureMgr);
	m_impl->m_textureManager->initTexture(m_impl->m_imgList);
	m_impl->m_imgList.clear();
	QOpenGLTexture *pTexture = m_impl->m_textureManager->getTexture(m_impl->m_textureManager->getCurIndex());
	if (pTexture)
	{
		m_impl->m_funcs->glActiveTexture(GL_TEXTURE0);
		m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, pTexture->textureId());
		m_impl->m_funcs->glActiveTexture(GL_TEXTURE1);
		m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, pTexture->textureId());
	}
	ShaderProgramManager *pProgramManager = new ShaderProgramManager(pContext);
	m_impl->m_programManager.reset(pProgramManager);
	m_impl->m_programManager->setFloatUniform(m_impl->m_TransitionType, "progress", 0.0f);
	m_impl->m_programManager->setIntUniform(m_impl->m_TransitionType, "texture0", 0);
	m_impl->m_programManager->setIntUniform(m_impl->m_TransitionType, "texture1", 1);

	float vertices[] = {
		// positions        / texture coords
		1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // top right
		1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
		-1.0f,  1.0f, 0.0f, 0.0f, 1.0f  // top left
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	m_impl->m_funcs->glGenVertexArrays(1, &m_impl->m_VAO);
	m_impl->m_funcs->glGenBuffers(1, &m_impl->m_VBO);
	m_impl->m_funcs->glGenBuffers(1, &m_impl->m_EBO);
	m_impl->m_funcs->glBindVertexArray(m_impl->m_VAO);

	m_impl->m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_impl->m_VBO);
	m_impl->m_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	m_impl->m_funcs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_impl->m_EBO);
	m_impl->m_funcs->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	m_impl->m_funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	m_impl->m_funcs->glEnableVertexAttribArray(0);
	m_impl->m_funcs->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	m_impl->m_funcs->glEnableVertexAttribArray(1);
	m_impl->m_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_impl->m_funcs->glBindVertexArray(0);

	connect(&m_impl->m_timer, &QTimer::timeout, this, &SceneTransition::onTimer);
	emit initialized();
}

void SceneTransition::paintGL()
{
	m_impl->m_funcs->glClear(GL_COLOR_BUFFER_BIT);
	m_impl->m_funcs->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	m_impl->m_funcs->glEnable(GL_BLEND);
	m_impl->m_funcs->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	unsigned int programIndex = m_impl->m_programManager->getTransitionShaderProgram(m_impl->m_TransitionType);
	m_impl->m_funcs->glUseProgram(programIndex);
	m_impl->m_funcs->glBindVertexArray(m_impl->m_VAO);
	m_impl->m_funcs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void SceneTransition::resizeGL(int w, int h)
{
	m_impl->m_funcs->glViewport(0, 0, w, h);
}

void SceneTransition::onTimer()
{
	makeCurrent();
	int elapsedTime = m_impl->m_elapsedTimer.elapsed();
	if (elapsedTime >= m_impl->m_duration)
	{
		m_impl->m_timer.stop();
		if (TransitionAction::Next == m_impl->m_action)
		{
			m_impl->m_funcs->glActiveTexture(GL_TEXTURE0);
			QOpenGLTexture *pNextTexture = m_impl->m_textureManager->getTexture(m_impl->m_textureManager->getCurIndex() + 1);
			m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, pNextTexture->textureId());
			int textureIndex = m_impl->m_textureManager->getCurIndex();
			m_impl->m_textureManager->setCurIndex(textureIndex + 1);
		}
		else if (TransitionAction::Prev == m_impl->m_action)
		{
			m_impl->m_funcs->glActiveTexture(GL_TEXTURE0);
			QOpenGLTexture *pPrevTexture = m_impl->m_textureManager->getTexture(m_impl->m_textureManager->getCurIndex() - 1);
			m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, pPrevTexture->textureId());
			int textureIndex = m_impl->m_textureManager->getCurIndex();
			m_impl->m_textureManager->setCurIndex(textureIndex - 1);
		}
		m_impl->m_action = TransitionAction::None;
		m_impl->m_funcs->glActiveTexture(GL_TEXTURE1);
		QOpenGLTexture *pTexture = m_impl->m_textureManager->getTexture(m_impl->m_textureManager->getCurIndex());
		m_impl->m_funcs->glBindTexture(GL_TEXTURE_2D, pTexture->textureId());
		m_impl->m_programManager->setFloatUniform(m_impl->m_TransitionType, "progress", 0.0f);
	}
	else
	{
		if (TransitionAction::Next == m_impl->m_action)
		{
			float ratio = (float)elapsedTime / (float)(m_impl->m_duration);
			m_impl->m_programManager->setFloatUniform(m_impl->m_TransitionType, "progress", ratio);
		}
		else if (TransitionAction::Prev == m_impl->m_action)
		{
			float ratio = (float)elapsedTime / (float)(m_impl->m_duration);
			m_impl->m_programManager->setFloatUniform(m_impl->m_TransitionType, "progress", 1.0 - ratio);
		}
	}
	doneCurrent();
	update();
}