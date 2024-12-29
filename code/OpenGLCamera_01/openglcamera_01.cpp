#include "openglcamera_01.h"
#include "camera.h"
#include "textureobject.h"
#include "shaderprogram.h"
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QDebug>

static const char *vertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;\n"

    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
    "   TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\0";

static const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D texture0;\n"
    "uniform sampler2D texture1;\n"
    "void main()\n"
    "{\n"
    "   FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2);\n"
    "}\0";

OpenGLCamera_01::OpenGLCamera_01(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(600, 500);
    setMouseTracking(true);
    setWindowTitle("LearnOpenGLInQt-OpenGLCamera_01");
}

OpenGLCamera_01::~OpenGLCamera_01()
{
    if (m_bInit)
    {
        m_funcs->glDeleteVertexArrays(1, &m_VAO);
        m_funcs->glDeleteBuffers(1, &m_VBO);
    }
}

void OpenGLCamera_01::initializeGL()
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
    m_funcs->glEnable(GL_DEPTH_TEST);

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

    Camera *pCamera = new Camera();
    m_camera.reset(pCamera);

    m_cubePositions.push_back(QVector3D(0.0f, 0.0f, 0.0f));
    m_cubePositions.push_back(QVector3D(2.0f, 5.0f, -15.0f));
    m_cubePositions.push_back(QVector3D(-1.5f, -2.2f, -2.5f));
    m_cubePositions.push_back(QVector3D(-3.8f, -2.0f, -12.3f));
    m_cubePositions.push_back(QVector3D(2.4f, -0.4f, -3.5f));
    m_cubePositions.push_back(QVector3D(-1.7f, 3.0f, -7.5f));
    m_cubePositions.push_back(QVector3D(1.3f, -2.0f, -2.5f));
    m_cubePositions.push_back(QVector3D(1.5f, 2.0f, -2.5f));
    m_cubePositions.push_back(QVector3D(1.5f, 0.2f, -1.5f));
    m_cubePositions.push_back(QVector3D(-1.3f, 1.0f, -1.5f));

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    m_funcs->glGenVertexArrays(1, &m_VAO);
    m_funcs->glGenBuffers(1, &m_VBO);

    m_funcs->glBindVertexArray(m_VAO);

    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    m_funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    m_funcs->glEnableVertexAttribArray(0);
    // texture coord attribute
    m_funcs->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    m_funcs->glEnableVertexAttribArray(1);

    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_funcs->glBindVertexArray(0);

    TextureObject *pTexture0 = new TextureObject(pContext, "img/01.jpg");
    TextureObject *pTexture1 = new TextureObject(pContext, "img/02.jpg");
    m_texture0.reset(pTexture0);
    m_texture1.reset(pTexture1);
}

void OpenGLCamera_01::paintGL()
{
    m_funcs->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    m_funcs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind textures on corresponding texture units
    m_funcs->glActiveTexture(GL_TEXTURE0);
    m_funcs->glBindTexture(GL_TEXTURE_2D, m_texture0->textureID());
    m_funcs->glActiveTexture(GL_TEXTURE1);
    m_funcs->glBindTexture(GL_TEXTURE_2D, m_texture1->textureID());

    // render container
    m_shaderProgram->use();

    QMatrix4x4 view;
    QMatrix4x4 projection;
    projection.perspective(m_camera->getZoom(), (float)width() / (float)height(), 0.1f, 100.0f);
    view = m_camera->getViewMatrix();
    m_shaderProgram->setMat4("view", view);
    m_shaderProgram->setMat4("projection", projection);

    m_funcs->glBindVertexArray(m_VAO);
    for (unsigned int i = 0; i < m_cubePositions.size(); i++)
    {
        QMatrix4x4 model;
        model.translate(m_cubePositions[i]);
        float angle = 20.0f * i;
        model.rotate(angle, QVector3D(1.0f, 0.3f, 0.5f));
        m_shaderProgram->setMat4("model", model);

        m_funcs->glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void OpenGLCamera_01::keyPressEvent(QKeyEvent *event)
{
    float deltaTime = 0.1f; // 这里先给个固定值
    if (event->key() == Qt::Key_W)
        m_camera->processKeyboard(Camera::FORWARD, deltaTime);
    if (event->key() == Qt::Key_S)
        m_camera->processKeyboard(Camera::BACKWARD, deltaTime);
    if (event->key() == Qt::Key_A)
        m_camera->processKeyboard(Camera::LEFT, deltaTime);
    if (event->key() == Qt::Key_D)
        m_camera->processKeyboard(Camera::RIGHT, deltaTime);
    update();
}

void OpenGLCamera_01::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bfirstMouse)
    {
        m_lastMouseX = event->x();
        m_lastMouseY = event->y();
        m_bfirstMouse = false;
    }

    float xoffset = event->x() - m_lastMouseX;
    float yoffset = m_lastMouseY - event->y();
    m_lastMouseX = event->x();
    m_lastMouseY = event->y();

    m_camera->processMouseMovement(xoffset, yoffset);
    update();
}

void OpenGLCamera_01::wheelEvent(QWheelEvent *event)
{
    float yoffset = event->angleDelta().y() * 0.05;
    m_camera->processMouseScroll(yoffset);
    update();
}