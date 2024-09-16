#include "coolprogress.h"
#include "progressshader.h"
#include <QOpenGLContext>
#include <QTimer>
#include <QElapsedTimer>
#include <QDebug>
#include <map>
#include <string>
#include "shaderprogram.h"

class CoolProgressImpl
{
public:
	CoolProgressImpl() {};
	~CoolProgressImpl() {};

public:
	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
	bool m_bInit = false;
	float m_speed = 1.0;
	float m_r1 = -1.0;
	float m_r2 = -1.0;
	std::map<int, std::string> vShaderMap;
	std::map<int, std::string> fShaderMap;
	CoolProgress::ProgressStyle m_style = CoolProgress::Ring_1;

	QTimer m_timer;
	QElapsedTimer m_elapsedTimer;

	QOpenGLFunctions_3_3_Core *m_funcs = nullptr;
    std::unique_ptr<ShaderProgram> m_shaderProgram;
};

CoolProgress::CoolProgress(bool bTransparent, ProgressStyle style, int width, int height, QWidget *parent)
	: QOpenGLWidget(parent)
{
	CoolProgressImpl *pImpl = new CoolProgressImpl();
	m_impl.reset(pImpl);
	m_impl->m_style = style;
	setFixedSize(width, height);
	if (bTransparent)
	{
		setAttribute(Qt::WA_TranslucentBackground, true);
		setAttribute(Qt::WA_AlwaysStackOnTop, true);
	}
	initShader();
}

CoolProgress::~CoolProgress()
{
    if (m_impl->m_bInit)
    {
        m_impl->m_funcs->glDeleteVertexArrays(1, &m_impl->m_VAO);
        m_impl->m_funcs->glDeleteBuffers(1, &m_impl->m_VBO);
    }
}

void CoolProgress::startProgress(float speed)
{
	m_impl->m_speed = speed;
	connect(&(m_impl->m_timer), &QTimer::timeout, this, &CoolProgress::updateProgress);
	m_impl->m_timer.start(10);
	m_impl->m_elapsedTimer.restart();
}

void CoolProgress::stopProgress()
{
	m_impl->m_timer.stop();
}

void CoolProgress::updateProgress()
{
	update();
}

void CoolProgress::setRingRadius(float r1, float r2)
{
	m_impl->m_r1 = r1;
	m_impl->m_r2 = r2;
	if (m_impl->m_bInit && 
		(m_impl->m_style == CoolProgress::Ring_1 ||
		m_impl->m_style == CoolProgress::Ring_2))
	{
		makeCurrent();
		m_impl->m_shaderProgram->use();
		m_impl->m_shaderProgram->setFloat("r1", r1);
		m_impl->m_shaderProgram->setFloat("r2", r2);
	}
}

void CoolProgress::initializeGL()
{
	if (m_impl->m_bInit)
		return;

	m_impl->m_bInit = true;
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

    ShaderProgram *pShaderProgram = new ShaderProgram(pContext);
    m_impl->m_shaderProgram.reset(pShaderProgram);
    const char *vertexShaderSource = m_impl->vShaderMap[m_impl->m_style].c_str();
    const char *fragmentShaderSource = m_impl->fShaderMap[m_impl->m_style].c_str();
    if (!m_impl->m_shaderProgram->compileSourceCode(vertexShaderSource, fragmentShaderSource))
    {
        Q_ASSERT(false);
        return;
    }

	float vertices[] = {
		// 第一个三角形
		1.0f, 1.0f, 0.0f,   // 右上角
		1.0f, -1.0f, 0.0f,  // 右下角
		-1.0f, 1.0f, 0.0f,  // 左上角
		// 第二个三角形
		1.0f, -1.0f, 0.0f,  // 右下角
		-1.0f, -1.0f, 0.0f, // 左下角
		-1.0f, 1.0f, 0.0f   // 左上角
	};

	m_impl->m_funcs->glGenVertexArrays(1, &m_impl->m_VAO);
	m_impl->m_funcs->glGenBuffers(1, &m_impl->m_VBO);
	m_impl->m_funcs->glBindVertexArray(m_impl->m_VAO);

	m_impl->m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_impl->m_VBO);
	m_impl->m_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	m_impl->m_funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	m_impl->m_funcs->glEnableVertexAttribArray(0);
	m_impl->m_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_impl->m_funcs->glBindVertexArray(0);

	setShaderUniform();
}

void CoolProgress::paintGL()
{
	m_impl->m_funcs->glClear(GL_COLOR_BUFFER_BIT);
	m_impl->m_funcs->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	m_impl->m_funcs->glEnable(GL_BLEND);
	m_impl->m_funcs->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_impl->m_shaderProgram->use();
	if (m_impl->m_timer.isActive())
	{
		qint64 time = m_impl->m_elapsedTimer.elapsed();
		float value = time / 1000.0 * m_impl->m_speed;
		m_impl->m_shaderProgram->setFloat("deltaTime", value);
	}
	m_impl->m_funcs->glBindVertexArray(m_impl->m_VAO);
	m_impl->m_funcs->glDrawArrays(GL_TRIANGLES, 0, 6);
}

void CoolProgress::resizeGL(int w, int h)
{
	m_impl->m_funcs->glViewport(0, 0, w, h);
}

void CoolProgress::initShader()
{
	m_impl->vShaderMap[CoolProgress::Ring_1] = vertexShaderSource_Ring_1;
	m_impl->fShaderMap[CoolProgress::Ring_1] = fragmentShaderSource_Ring_1;
	m_impl->vShaderMap[CoolProgress::Ring_2] = vertexShaderSource_Ring_2;
	m_impl->fShaderMap[CoolProgress::Ring_2] = fragmentShaderSource_Ring_2;
	m_impl->vShaderMap[CoolProgress::Ring_3] = vertexShaderSource_Ring_3;
	m_impl->fShaderMap[CoolProgress::Ring_3] = fragmentShaderSource_Ring_3;
	m_impl->vShaderMap[CoolProgress::FlashDot_1] = vertexShaderSource_FlashDot_1;
	m_impl->fShaderMap[CoolProgress::FlashDot_1] = fragmentShaderSource_FlashDot_1;
	m_impl->vShaderMap[CoolProgress::FlashDot_2] = vertexShaderSource_FlashDot_2;
	m_impl->fShaderMap[CoolProgress::FlashDot_2] = fragmentShaderSource_FlashDot_2;
	m_impl->vShaderMap[CoolProgress::FlashDot_3] = vertexShaderSource_FlashDot_3;
	m_impl->fShaderMap[CoolProgress::FlashDot_3] = fragmentShaderSource_FlashDot_3;
	m_impl->vShaderMap[CoolProgress::FlashDot_4] = vertexShaderSource_FlashDot_4;
	m_impl->fShaderMap[CoolProgress::FlashDot_4] = fragmentShaderSource_FlashDot_4;
	m_impl->vShaderMap[CoolProgress::Rect_1] = vertexShaderSource_Rect_1;
	m_impl->fShaderMap[CoolProgress::Rect_1] = fragmentShaderSource_Rect_1;
	m_impl->vShaderMap[CoolProgress::Rect_2] = vertexShaderSource_Rect_2;
	m_impl->fShaderMap[CoolProgress::Rect_2] = fragmentShaderSource_Rect_2;
	m_impl->vShaderMap[CoolProgress::Rect_3] = vertexShaderSource_Rect_3;
	m_impl->fShaderMap[CoolProgress::Rect_3] = fragmentShaderSource_Rect_3;
	m_impl->vShaderMap[CoolProgress::WaterWave_1] = vertexShaderSource_WaterWave_1;
	m_impl->fShaderMap[CoolProgress::WaterWave_1] = fragmentShaderSource_WaterWave_1;
	m_impl->vShaderMap[CoolProgress::Polygon_1] = vertexShaderSource_Polygon_1;
	m_impl->fShaderMap[CoolProgress::Polygon_1] = fragmentShaderSource_Polygon_1;
}

void CoolProgress::setShaderUniform()
{
	m_impl->m_shaderProgram->use();
	if (m_impl->m_style == CoolProgress::Ring_1)
	{
		float r1 = 0.3;
		float r2 = 0.35;
		if (m_impl->m_r1 > 0.0 && m_impl->m_r2 > 0.0)
		{
			r1 = m_impl->m_r1;
			r2 = m_impl->m_r2;
		}
		m_impl->m_shaderProgram->setFloat("r1", r1);
		m_impl->m_shaderProgram->setFloat("r2", r2);
	}
	if (m_impl->m_style == CoolProgress::Ring_2)
	{
		float r1 = 0.14;
		float r2 = 0.148;
		if (m_impl->m_r1 > 0.0 && m_impl->m_r2 > 0.0)
		{
			r1 = m_impl->m_r1;
			r2 = m_impl->m_r2;
		}
		m_impl->m_shaderProgram->setFloat("r1", r1);
		m_impl->m_shaderProgram->setFloat("r2", r2);
	}
}