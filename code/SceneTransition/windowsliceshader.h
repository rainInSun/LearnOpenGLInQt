/*
Author: gre
License: MIT
*/

static const char *windowSliceVertexSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout(location = 1) in vec2 aTexCoord;\n"
	"out vec2 texCoord;\n"
	"void main()\n"
	"{\n"
	"	texCoord = aTexCoord;\n"
	"	gl_Position = vec4(aPos, 1.0);\n"
	"}\0";

static const char *windowSliceFragmentSource = "#version 330 core\n"
	"const float count = 10.0;\n"
	"const float smoothness = 0.5;\n"
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
	"	float pr = smoothstep(-smoothness, 0.0, p.x - progress * (1.0 + smoothness));\n"
	"	float s = step(pr, fract(count * p.x));\n"
	"	fragColor = mix(texture(texture0, p), texture(texture1, p), s);\n"
	"}\0";