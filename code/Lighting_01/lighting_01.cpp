#include "lighting_01.h"
#include "camera.h"
#include "shaderprogram.h"
#include <QMatrix4x4>
#include <QKeyEvent>
#include <QDebug>

static const char *lightVertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aNormal;\n"
    "out vec3 FragPos;\n"
    "out vec3 Normal;\n"

    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
    "   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
    "   gl_Position = projection * view * vec4(FragPos, 1.0f);\n"
    "}\0";

static const char *lightFragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"

    "in vec3 Normal;\n"
    "in vec3 FragPos;\n"
    
    "uniform vec3 lightPos;\n"
    "uniform vec3 viewPos;\n"
    "uniform vec3 lightColor;\n"
    "uniform vec3 objectColor;\n"

    "void main()\n"
    "{\n"
    "    // ambient\n"
    "    float ambientStrength = 0.1;\n"
    "    vec3 ambient = ambientStrength * lightColor;\n"
        
    "    // diffuse\n"
    "    vec3 norm = normalize(Normal);\n"
    "    vec3 lightDir = normalize(lightPos - FragPos);\n"
    "    float diff = max(dot(norm, lightDir), 0.0);\n"
    "    vec3 diffuse = diff * lightColor;\n"
        
    "    // specular\n"
    "    float specularStrength = 0.5;\n"
    "    vec3 viewDir = normalize(viewPos - FragPos);\n"
    "    vec3 reflectDir = reflect(-lightDir, norm);\n"
    "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
    "    vec3 specular = specularStrength * spec * lightColor;\n"
            
    "    vec3 result = (ambient + diffuse + specular) * objectColor;\n"
    "    FragColor = vec4(result, 1.0);\n"
    "}\0";

static const char *lightSourceVertexShaderSource ="#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"

    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"

    "void main()\n"
    "{\n"
    "    gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "}\0";

static const char *lightSourceFragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"

    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0); // set all 4 vector values to 1.0\n"
    "}\0";

Lighting_01::Lighting_01(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setFixedSize(600, 500);
    setMouseTracking(true);
    setWindowTitle("LearnOpenGLInQt-Lighting_01");
}

Lighting_01::~Lighting_01()
{
    if (m_bInit)
    {
        m_funcs->glDeleteVertexArrays(1, &m_cubeVAO);
        m_funcs->glDeleteVertexArrays(1, &m_lightSourceVAO);
        m_funcs->glDeleteBuffers(1, &m_VBO);
    }
}

void Lighting_01::initializeGL()
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

    ShaderProgram *pLightShaderProgram = new ShaderProgram(pContext);
    m_lightShaderProgram.reset(pLightShaderProgram);
    if (!m_lightShaderProgram->compileSourceCode(lightVertexShaderSource, lightFragmentShaderSource))
    {
        Q_ASSERT(false);
        return;
    }

    ShaderProgram *pLightSourceShaderProgram = new ShaderProgram(pContext);
    m_lightSourceShaderProgram.reset(pLightSourceShaderProgram);
    if (!m_lightSourceShaderProgram->compileSourceCode(lightSourceVertexShaderSource, lightSourceFragmentShaderSource))
    {
        Q_ASSERT(false);
        return;
    }

    Camera *pCamera = new Camera();
    m_camera.reset(pCamera);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    m_funcs->glGenVertexArrays(1, &m_cubeVAO);
    m_funcs->glGenBuffers(1, &m_VBO);

    m_funcs->glBindVertexArray(m_cubeVAO);

    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_funcs->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    m_funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    m_funcs->glEnableVertexAttribArray(0);
    // texture coord attribute
    m_funcs->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    m_funcs->glEnableVertexAttribArray(1);

    m_funcs->glGenVertexArrays(1, &m_lightSourceVAO);
    m_funcs->glBindVertexArray(m_lightSourceVAO);
    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    m_funcs->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    m_funcs->glEnableVertexAttribArray(0);

    m_funcs->glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_funcs->glBindVertexArray(0);
}

void Lighting_01::paintGL()
{
    m_funcs->glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    m_funcs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // render container
    m_lightShaderProgram->use();

    m_lightShaderProgram->setVector3f("objectColor", 1.0f, 0.5f, 0.31f);
    m_lightShaderProgram->setVector3f("lightColor", 1.0f, 1.0f, 1.0f);
    m_lightShaderProgram->setVector3f("lightPos", m_lightPos);
    m_lightShaderProgram->setVector3f("viewPos", m_camera->getPosition());

    QMatrix4x4 view;
    QMatrix4x4 projection;
    projection.perspective(m_camera->getZoom(), (float)width() / (float)height(), 0.1f, 100.0f);
    view = m_camera->getViewMatrix();
    m_lightShaderProgram->setMat4("view", view);
    m_lightShaderProgram->setMat4("projection", projection);
    QMatrix4x4 model;
    m_lightShaderProgram->setMat4("model", model);

    m_funcs->glBindVertexArray(m_cubeVAO);
    m_funcs->glDrawArrays(GL_TRIANGLES, 0, 36);

    m_lightSourceShaderProgram->use();
    m_lightSourceShaderProgram->setMat4("view", view);
    m_lightSourceShaderProgram->setMat4("projection", projection);
    QMatrix4x4 lightModel;
    lightModel.translate(m_lightPos);
    lightModel.scale(0.2f, 0.2f, 0.2f);
    m_lightSourceShaderProgram->setMat4("model", lightModel);

    m_funcs->glBindVertexArray(m_lightSourceVAO);
    m_funcs->glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Lighting_01::keyPressEvent(QKeyEvent *event)
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

void Lighting_01::mouseMoveEvent(QMouseEvent *event)
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

void Lighting_01::wheelEvent(QWheelEvent *event)
{
    float yoffset = event->angleDelta().y() * 0.05;
    m_camera->processMouseScroll(yoffset);
    update();
}