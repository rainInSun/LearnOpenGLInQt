#include "glsl_01.h"
#include "shader.h"
#include <iostream>
#include <QDebug>

static const char *vertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "}\0";

static const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

GLSL_01::GLSL_01(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(600, 500);
    setWindowTitle("LearnOpenGLInQt-GLSL_01");
    m_elapsedTimer.start();
    connect(&m_timer, &QTimer::timeout, this, [=](){
        update(); // 这里需要一直触发界面刷新，调用paintGL()函数，否则界面不会一直刷新
    });
    m_timer.start();
}

GLSL_01::~GLSL_01()
{
}

void GLSL_01::initializeGL()
{
    if (m_bInit)
        return;

    m_bInit = true;
	QOpenGLContext *pContext = context();
	if (pContext)
		m_funcs = pContext->versionFunctions<QOpenGLFunctions_3_3_Core>();
    if (!m_funcs)
        return;

    /*// 顶点着色器
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
    */
    Shader *pShader = new Shader(pContext);
    m_shader.reset(pShader);
    m_shader->compileSourceCode(vertexShaderSource, fragmentShaderSource);

    // 三角形顶点
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    m_funcs->glGenVertexArrays(1, &m_VAO);
    m_funcs->glGenBuffers(1, &m_VBO);
    // 绑定VAO，配置顶点属性
    m_funcs->glBindVertexArray(m_VAO);

    // 绑定VBO，配置数据
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    m_funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    m_funcs->glEnableVertexAttribArray(0);

    // 解绑VBO、VAO，防止误操作
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0); 
    m_funcs->glBindVertexArray(0); 
}

void GLSL_01::paintGL()
{
    m_funcs->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    m_funcs->glClear(GL_COLOR_BUFFER_BIT);

    // 指定着色器
    m_shader->use();
    // 更新uniform颜色
    qint64 time = m_elapsedTimer.elapsed();
    float timeValue = (float)time / 1000.0;
    float greenValue = sin(timeValue) / 2.0f + 0.5f;
    qDebug() << "greenValue: " << greenValue;
    /*
    int vertexColorLocation = m_funcs->glGetUniformLocation(m_program, "ourColor");
    m_funcs->glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    */
    m_shader->setVector4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

    // 绑定VAO
    m_funcs->glBindVertexArray(m_VAO);
    // 绘制三角形
    m_funcs->glDrawArrays(GL_TRIANGLES, 0, 3);
}