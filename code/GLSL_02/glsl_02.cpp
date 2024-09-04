#include "glsl_02.h"
#include <iostream>

static const char *vertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   ourColor = aColor;\n"
    "}\0";

static const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(ourColor, 1.0f);\n"
    "}\n\0";

GLSL_02::GLSL_02(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(600, 500);
    setWindowTitle("LearnOpenGLInQt-GLSL_02");
}

GLSL_02::~GLSL_02()
{
}

void GLSL_02::initializeGL()
{
    if (m_bInit)
        return;

    m_bInit = true;
	QOpenGLContext *pContext = context();
	if (pContext)
		m_funcs = pContext->versionFunctions<QOpenGLFunctions_3_3_Core>();
    if (!m_funcs)
        return;

    // 顶点着色器
    unsigned int vertexShader = m_funcs->glCreateShader(GL_VERTEX_SHADER);
    m_funcs->glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    m_funcs->glCompileShader(vertexShader);
    // 着色器编译
    int success;
    char infoLog[512];
    m_funcs->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // 检查编译错误
    if (!success)
    {
        m_funcs->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // 片段着色器
    unsigned int fragmentShader = m_funcs->glCreateShader(GL_FRAGMENT_SHADER);
    m_funcs->glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    m_funcs->glCompileShader(fragmentShader);
    m_funcs->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    // 检查编译错误
    if (!success)
    {
        m_funcs->glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // 链接着色器
    m_program = m_funcs->glCreateProgram();
    m_funcs->glAttachShader(m_program, vertexShader);
    m_funcs->glAttachShader(m_program, fragmentShader);
    m_funcs->glLinkProgram(m_program);
    // 检查链接错误
    m_funcs->glGetProgramiv(m_program, GL_LINK_STATUS, &success);
    if (!success) {
        m_funcs->glGetProgramInfoLog(m_program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    // 链接成功后，保留可以删除着色器
    m_funcs->glDeleteShader(vertexShader);
    m_funcs->glDeleteShader(fragmentShader);

    // 三角形顶点
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };

    m_funcs->glGenVertexArrays(1, &m_VAO);
    m_funcs->glGenBuffers(1, &m_VBO);
    // 绑定VAO，配置顶点属性
    m_funcs->glBindVertexArray(m_VAO);

    // 绑定VBO，配置数据
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 位置属性
    m_funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    m_funcs->glEnableVertexAttribArray(0);
    // 颜色属性
    m_funcs->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    m_funcs->glEnableVertexAttribArray(1);

    // 解绑VBO、VAO，防止误操作
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0); 
    m_funcs->glBindVertexArray(0); 
}

void GLSL_02::paintGL()
{
    m_funcs->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    m_funcs->glClear(GL_COLOR_BUFFER_BIT);

    // 指定着色器
    m_funcs->glUseProgram(m_program);

    // 绑定VAO
    m_funcs->glBindVertexArray(m_VAO);
    // 绘制三角形
    m_funcs->glDrawArrays(GL_TRIANGLES, 0, 3);
}