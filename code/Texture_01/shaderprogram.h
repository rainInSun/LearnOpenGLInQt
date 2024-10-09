#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>
#include <QOpenGLShader>
#include <QFile>
#include <QTextStream>
#include <QVector2D>
#include <QVector3D>
#include <string>

class ShaderProgram
{
public:
    ShaderProgram(QOpenGLContext *pContext)
    {
        m_funcs = pContext->versionFunctions<QOpenGLFunctions_3_3_Core>();
        if (!m_funcs)
        {
            qWarning() << "Could not obtain required OpenGL context version";
            Q_ASSERT(false);
        }
    }
    ~ShaderProgram()
    {
        if (0 != m_ID)
            m_funcs->glDeleteProgram(m_ID);
    }

    bool compileSourceCode(const char *vertexCode, const char *fragmentCode)
    {
        int success = -1;
        char infoLog[512];
        // vertex shader
        unsigned int vertexShader = m_funcs->glCreateShader(GL_VERTEX_SHADER);
        m_funcs->glShaderSource(vertexShader, 1, &vertexCode, NULL);
        m_funcs->glCompileShader(vertexShader);
        m_funcs->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            m_funcs->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            qWarning() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
            Q_ASSERT(false);
            return false;
        }
        // fragment shader
        unsigned int fragmentShader = m_funcs->glCreateShader(GL_FRAGMENT_SHADER);
        m_funcs->glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
        m_funcs->glCompileShader(fragmentShader);
        m_funcs->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            m_funcs->glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            qWarning() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
            Q_ASSERT(false);
            return false;
        }
        // link shaders
        unsigned int shaderProgram = m_funcs->glCreateProgram();
        m_funcs->glAttachShader(shaderProgram, vertexShader);
        m_funcs->glAttachShader(shaderProgram, fragmentShader);
        m_funcs->glLinkProgram(shaderProgram);
        // check for linking errors
        m_funcs->glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success)
        {
            m_funcs->glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            qWarning() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
            Q_ASSERT(false);
            return false;
        }
        m_funcs->glDeleteShader(vertexShader);
        m_funcs->glDeleteShader(fragmentShader);
        m_ID = shaderProgram;
        return true;
    }

    bool compileSourceFile(const QString& vertexFileName, const QString& fragmentFileName)
    {
        QFile vertexFile(vertexFileName);
        if (!vertexFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qWarning() << "Open vertexFile Failed";
            Q_ASSERT(false);
            return false;
        }
        QFile fragmentFile(fragmentFileName);
        if (!fragmentFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qWarning() << "Open fragmentFile Failed";
            Q_ASSERT(false);
            return false;
        }

        QTextStream vStream(&vertexFile);
        QString vertexStr = vStream.readAll();

        QTextStream fStream(&fragmentFile);
        QString fragmentStr = fStream.readAll();
        vertexFile.close();
        fragmentFile.close();
        std::string vStr = vertexStr.toStdString();
        std::string fStr = fragmentStr.toStdString();

        return compileSourceCode(vStr.c_str(), fStr.c_str());
    }

    void use()
    {
        m_funcs->glUseProgram(m_ID);
    }

	void setFloat(const char *name, float value, bool useShader = false)
    {
        if (useShader)
            use();

        m_funcs->glUniform1f(m_funcs->glGetUniformLocation(m_ID, name), value);
    }

	void setInteger(const char *name, int value, bool useShader = false)
    {
        if (useShader)
            use();

        m_funcs->glUniform1i(m_funcs->glGetUniformLocation(m_ID, name), value); 
    }

	void setVector2f(const char *name, float x, float y, bool useShader = false)
    {
        if (useShader)
            use();

        m_funcs->glUniform2f(m_funcs->glGetUniformLocation(m_ID, name), x, y);
    }

	void setVector2f(const char *name, const QVector2D &value, bool useShader = false)
    {
        if (useShader)
            use();

        m_funcs->glUniform2f(m_funcs->glGetUniformLocation(m_ID, name), value.x(), value.y());
    }

	void setVector3f(const char *name, float x, float y, float z, bool useShader = false)
    {
        if (useShader)
            use();

        m_funcs->glUniform3f(m_funcs->glGetUniformLocation(m_ID, name), x, y, z);
    }

	void setVector3f(const char *name, const QVector3D &value, bool useShader = false)
    {
        if (useShader)
            use();

         m_funcs->glUniform3f(m_funcs->glGetUniformLocation(m_ID, name), value.x(), value.y(), value.z()); 
    }
	void setVector4f(const char *name, float x, float y, float z, float w, bool useShader = false)
    {
        if (useShader)
            use();

        m_funcs->glUniform4f(m_funcs->glGetUniformLocation(m_ID, name), x, y, z, w);
    }
	void setVector4f(const char *name, const QVector4D &value, bool useShader = false)
    {
        if (useShader)
            use();

        m_funcs->glUniform4f(m_funcs->glGetUniformLocation(m_ID, name), value.x(), value.y(), value.z(), value.w());
    }

private:
    unsigned int m_ID = 0;
    QOpenGLFunctions_3_3_Core *m_funcs = nullptr;
};

#endif
