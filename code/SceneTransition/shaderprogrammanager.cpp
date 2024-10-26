#include "shaderprogrammanager.h"
#include "shadersource.h"
#include <QDebug>

ShaderProgramManager::ShaderProgramManager(QOpenGLContext *pContext)
{
	m_funcs = pContext->versionFunctions<QOpenGLFunctions_3_3_Core>();
	if (!m_funcs)
	{
		qWarning() << "Could not obtain required OpenGL context version";
		Q_ASSERT(false);
	}
	initShaderSource();
}

ShaderProgramManager::~ShaderProgramManager()
{
}

void ShaderProgramManager::initShaderSource()
{
	m_vertexSourceMap[TransitionType::Fade] = fadeVertexSource;
	m_fragSourceMap[TransitionType::Fade] = fadeFragmentSource;
	m_vertexSourceMap[TransitionType::BowTieHoriz] = bowTieHorizVertexSource;
	m_fragSourceMap[TransitionType::BowTieHoriz] = bowTieHorizFragmentSource;
	m_vertexSourceMap[TransitionType::PageCurl] = pageCurlVertexSource;
	m_fragSourceMap[TransitionType::PageCurl] = pageCurlFragmentSource;
	m_vertexSourceMap[TransitionType::LuminanceMelt] = luminanceMeltVertexSource;
	m_fragSourceMap[TransitionType::LuminanceMelt] = LuminanceMeltFragmentSource;
	m_vertexSourceMap[TransitionType::Perlin] = perlinVertexSource;
	m_fragSourceMap[TransitionType::Perlin] = perlinFragmentSource;
	m_vertexSourceMap[TransitionType::WipeRight] = wipeRightVertexSource;
	m_fragSourceMap[TransitionType::WipeRight] = wipeRightFragmentSource;
	m_vertexSourceMap[TransitionType::WipeLeft] = wipeLeftVertexSource;
	m_fragSourceMap[TransitionType::WipeLeft] = wipeLeftFragmentSource;
	m_vertexSourceMap[TransitionType::GridFlip] = gridFlipVertexSource;
	m_fragSourceMap[TransitionType::GridFlip] = gridFlipFragmentSource;
	m_vertexSourceMap[TransitionType::WindowSlice] = windowSliceVertexSource;
	m_fragSourceMap[TransitionType::WindowSlice] = windowSliceFragmentSource;
	m_vertexSourceMap[TransitionType::WaterDrop] = waterDropVertexSource;
	m_fragSourceMap[TransitionType::WaterDrop] = waterDropFragmentSource;
	m_vertexSourceMap[TransitionType::Doorway] = doorwayVertexSource;
	m_fragSourceMap[TransitionType::Doorway] = doorwayFragmentSource;
	m_vertexSourceMap[TransitionType::Cube] = cubeVertexSource;
	m_fragSourceMap[TransitionType::Cube] = cubeFragmentSource;
}

unsigned int ShaderProgramManager::getTransitionShaderProgram(TransitionType type)
{
	auto it = m_programMap.find((unsigned int)type);
	if (it != m_programMap.end())
		return it->second;

	int success = -1;
	char infoLog[512];
	// vertex shader
	unsigned int vertexShader = m_funcs->glCreateShader(GL_VERTEX_SHADER);
	const char* vStr = m_vertexSourceMap[type].c_str();
	m_funcs->glShaderSource(vertexShader, 1, &vStr, NULL);
	m_funcs->glCompileShader(vertexShader);
	m_funcs->glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		m_funcs->glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		qDebug() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog;
		Q_ASSERT(false);
	}
	// fragment shader
	unsigned int fragmentShader = m_funcs->glCreateShader(GL_FRAGMENT_SHADER);
	const char* fStr = m_fragSourceMap[type].c_str();
	m_funcs->glShaderSource(fragmentShader, 1, &fStr, NULL);
	m_funcs->glCompileShader(fragmentShader);
	m_funcs->glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		m_funcs->glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		qDebug() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog;
		Q_ASSERT(false);
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
		qDebug() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog;
		Q_ASSERT(false);
	}
	m_funcs->glDeleteShader(vertexShader);
	m_funcs->glDeleteShader(fragmentShader);

	m_programMap[type] = shaderProgram;
	return shaderProgram;
}

bool ShaderProgramManager::setIntUniform(TransitionType type, const std::string& name, int value)
{
	unsigned int programIndex = getTransitionShaderProgram(type);
	m_funcs->glUseProgram(programIndex);
	m_funcs->glUniform1i(m_funcs->glGetUniformLocation(programIndex, name.c_str()), value);
	return true;
}

bool ShaderProgramManager::setFloatUniform(TransitionType type, const std::string& name, float value)
{
	unsigned int programIndex = getTransitionShaderProgram(type);
	m_funcs->glUseProgram(programIndex);
	m_funcs->glUniform1f(m_funcs->glGetUniformLocation(programIndex, name.c_str()), value);
	return true;
}