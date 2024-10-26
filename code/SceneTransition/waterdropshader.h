/*
author: Paweł Płóciennik
license: MIT
*/

static const char *waterDropVertexSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout(location = 1) in vec2 aTexCoord;\n"
	"out vec2 texCoord;\n"
	"void main()\n"
	"{\n"
	"	texCoord = aTexCoord;\n"
	"	gl_Position = vec4(aPos, 1.0);\n"
	"}\0";

static const char *waterDropFragmentSource = "#version 330 core\n"
	"const float amplitude = 30;\n"
	"const float speed = 30;\n"
	"\n"
	"out vec4 fragColor;\n"
	"in vec2 texCoord;\n"
	"uniform float progress;\n"
	"// texture samplers\n"
	"uniform sampler2D texture0;\n"
	"uniform sampler2D texture1;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	vec2 p = texCoord.xy;\n"
	"	vec2 dir = p - vec2(.5);\n"
	"	float dist = length(dir);\n"
	"\n"
	"	if (dist > progress) {\n"
	"		fragColor = mix(texture(texture0, p), texture(texture1, p), progress);\n"
	"	} else {\n"
	"		vec2 offset = dir * sin(dist * amplitude - progress * speed);\n"
	"		fragColor = mix(texture(texture0, p + offset), texture(texture1, p), progress);\n"
	"	}\n"
	"}\0";