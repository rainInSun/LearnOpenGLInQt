/*
Author: Jake Nelson
License: MIT
*/

static const char *wipeRightVertexSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout(location = 1) in vec2 aTexCoord;\n"
	"out vec2 texCoord;\n"
	"void main()\n"
	"{\n"
	"	texCoord = aTexCoord;\n"
	"	gl_Position = vec4(aPos, 1.0);\n"
	"}\0";

static const char *wipeRightFragmentSource = "#version 330 core\n"
	"out vec4 fragColor;\n"
	"in vec2 texCoord;\n"
	"uniform float progress;\n"
	"// texture samplers\n"
	"uniform sampler2D texture0;\n"
	"uniform sampler2D texture1;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	vec2 uv = texCoord.xy;\n"
	"	vec2 p=uv.xy/vec2(1.0).xy;\n"
	"	vec4 a=texture(texture0,p);\n"
	"	vec4 b=texture(texture1,p);\n"
	"	fragColor = mix(a, b, step(0.0+p.x,progress));\n"
	"}\0";

static const char *wipeLeftVertexSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout(location = 1) in vec2 aTexCoord;\n"
	"out vec2 texCoord;\n"
	"void main()\n"
	"{\n"
	"	texCoord = aTexCoord;\n"
	"	gl_Position = vec4(aPos, 1.0);\n"
	"}\0";

static const char *wipeLeftFragmentSource = "#version 330 core\n"
	"out vec4 fragColor;\n"
	"in vec2 texCoord;\n"
	"uniform float progress;\n"
	"// texture samplers\n"
	"uniform sampler2D texture0;\n"
	"uniform sampler2D texture1;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	vec2 uv = texCoord.xy;\n"
	"	vec2 p=uv.xy/vec2(1.0).xy;\n"
	"	vec4 a=texture(texture0,p);\n"
	"	vec4 b=texture(texture1,p);\n"
	"	fragColor = mix(a, b, step(1.0-p.x,progress));\n"
	"}\0";