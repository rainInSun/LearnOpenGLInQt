#include "hellotriangle_01.h"
#include <iostream>

// 先不用管，后面会讲解着色器
static const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
static const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

HelloTriangle_01::HelloTriangle_01(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(600, 500);
    setWindowTitle("LearnOpenGLInQt-HelloTriangle_01");
}

HelloTriangle_01::~HelloTriangle_01()
{
}

void HelloTriangle_01::initializeGL()
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
    // float vertices[] = {
    //     -0.5f, -0.5f, 0.0f, // left  
    //      0.5f, -0.5f, 0.0f, // right 
    //      0.0f,  0.5f, 0.0f  // top   
    // }; 

    // 正方形顶点
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    m_funcs->glGenVertexArrays(1, &m_VAO);
    m_funcs->glGenBuffers(1, &m_VBO);
    m_funcs->glGenBuffers(1, &m_EBO);
    // 绑定VAO，配置顶点属性
    m_funcs->glBindVertexArray(m_VAO);

    // 绑定VBO，配置数据
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 绑定EBO，配置数据
    m_funcs->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    m_funcs->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    m_funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    m_funcs->glEnableVertexAttribArray(0);

    // 解绑VBO、VAO，防止误操作
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0); 
    m_funcs->glBindVertexArray(0); 
}

void HelloTriangle_01::paintGL()
{
    m_funcs->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    m_funcs->glClear(GL_COLOR_BUFFER_BIT);

    // 指定着色器
    m_funcs->glUseProgram(m_program);
    // 绑定VAO
    m_funcs->glBindVertexArray(m_VAO);
    // 绘制三角形
    //m_funcs->glDrawArrays(GL_TRIANGLES, 0, 3);
    // 通过EBO方式绘制正方形
    m_funcs->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}