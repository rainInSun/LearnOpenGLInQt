#ifndef SHADERPROGRAM_MANAGER_H
#define SHADERPROGRAM_MANAGER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLContext>
#include <map>
#include "scenetransitiondef.h"

class ShaderProgramManager
{
public:
	ShaderProgramManager(QOpenGLContext *pContext);
	~ShaderProgramManager();

	unsigned int getTransitionShaderProgram(TransitionType type);
	bool setIntUniform(TransitionType type, const std::string& name, int value);
	bool setFloatUniform(TransitionType type, const std::string& name, float value);

private:
	void initShaderSource();

private:
	std::map<unsigned int, unsigned int> m_programMap;
	std::map<unsigned int, std::string> m_vertexSourceMap;
	std::map<unsigned int, std::string> m_fragSourceMap;
	QOpenGLFunctions_3_3_Core *m_funcs = nullptr;
};
#endif