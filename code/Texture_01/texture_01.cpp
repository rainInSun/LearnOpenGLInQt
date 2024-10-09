#include "texture_01.h"
#include "textureobject.h"
#include "shaderprogram.h"
#include <QDebug>

static const char *vertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "out vec3 ourColor;\n"
    "out vec2 TexCoord;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "   TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\0";

static const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D texture0;\n"
    "uniform sampler2D texture1;\n"
    "void main()\n"
    "{\n"
    "   // linearly interpolate between both textures (80% container, 20% awesomeface)\n"
    "   FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.5);\n"
    "}\0";

Texture_01::Texture_01(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(600, 500);
    setWindowTitle("LearnOpenGLInQt-Texture_01");
}

Texture_01::~Texture_01()
{
    if (m_bInit)
    {
        m_funcs->glDeleteVertexArrays(1, &m_VAO);
        m_funcs->glDeleteBuffers(1, &m_VBO);
    }
}

void Texture_01::initializeGL()
{
	if (m_bInit)
		return;

	m_bInit = true;
	makeCurrent();
	QOpenGLContext *pContext = context();
	if (pContext)
		m_funcs = pContext->versionFunctions<QOpenGLFunctions_3_3_Core>();
	if (!m_funcs)
	{
		qWarning() << "Could not obtain required OpenGL context version";
		Q_ASSERT(false);
		return;
	}
	m_funcs->glViewport(0, 0, width(), height());

    ShaderProgram *pShaderProgram = new ShaderProgram(pContext);
    m_shaderProgram.reset(pShaderProgram);
    if (!m_shaderProgram->compileSourceCode(vertexShaderSource, fragmentShaderSource))
    {
        Q_ASSERT(false);
        return;
    }
    m_shaderProgram->use();
    m_shaderProgram->setInteger("texture0", 0);
    m_shaderProgram->setInteger("texture1", 1);

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

	m_funcs->glGenVertexArrays(1, &m_VAO);
	m_funcs->glGenBuffers(1, &m_VBO);
	m_funcs->glGenBuffers(1, &m_EBO);

    m_funcs->glBindVertexArray(m_VAO);

	m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	m_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    m_funcs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    m_funcs->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    m_funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    m_funcs->glEnableVertexAttribArray(0);
    // color attribute
    m_funcs->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    m_funcs->glEnableVertexAttribArray(1);
    // texture coord attribute
    m_funcs->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    m_funcs->glEnableVertexAttribArray(2);

	m_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0);
	m_funcs->glBindVertexArray(0);

    TextureObject *pTexture0 = new TextureObject(pContext, "img/01.jpg");
    TextureObject *pTexture1 = new TextureObject(pContext, "img/02.jpg");
    m_texture0.reset(pTexture0);
    m_texture1.reset(pTexture1);
}

void Texture_01::paintGL()
{
    m_funcs->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    m_funcs->glClear(GL_COLOR_BUFFER_BIT);

    // bind textures on corresponding texture units
    m_funcs->glActiveTexture(GL_TEXTURE0);
    m_funcs->glBindTexture(GL_TEXTURE_2D, m_texture0->textureID());
    m_funcs->glActiveTexture(GL_TEXTURE1);
    m_funcs->glBindTexture(GL_TEXTURE_2D, m_texture1->textureID());

    // render container
    m_shaderProgram->use();
    m_funcs->glBindVertexArray(m_VAO);
    m_funcs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}