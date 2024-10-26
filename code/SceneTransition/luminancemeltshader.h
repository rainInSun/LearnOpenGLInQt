﻿/*
Author: 0gust1
License: MIT
*/

static const char *luminanceMeltVertexSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout(location = 1) in vec2 aTexCoord;\n"
	"out vec2 texCoord;\n"
	"void main()\n"
	"{\n"
	"	texCoord = aTexCoord;\n"
	"	gl_Position = vec4(aPos, 1.0);\n"
	"}\0";

static const char *LuminanceMeltFragmentSource = "#version 330 core\n"
	"out vec4 fragColor;\n"
	"in vec2 texCoord;\n"
	"uniform float progress;\n"
	"// texture samplers\n"
	"uniform sampler2D texture0;\n"
	"uniform sampler2D texture1;\n"
	"\n"
	"//direction of movement :  0 : up, 1, down\n"
	"const int direction = 1; // = 1\n"
	"//luminance threshold\n"
	"const float l_threshold = 0.8; // = 0.8\n"
	"//does the movement takes effect above or below luminance threshold ?\n"
	"const bool above = false; // = false\n"
	"\n"
	"//Random function borrowed from everywhere\n"
	"float rand(vec2 co)\n"
	"{\n"
	"	return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);\n"
	"}\n"
	"\n"
	"vec3 mod289(vec3 x)\n"
	"{\n"
	"	return x - floor(x * (1.0 / 289.0)) * 289.0;\n"
	"}\n"
	"\n"
	"vec2 mod289(vec2 x)\n"
	"{\n"
	"	return x - floor(x * (1.0 / 289.0)) * 289.0;\n"
	"}\n"
	"\n"
	"vec3 permute(vec3 x)\n"
	"{\n"
	"	return mod289(((x*34.0)+1.0)*x);\n"
	"}\n"
	"\n"
	"float snoise(vec2 v)\n"
	"{\n"
	"	const vec4 C = vec4(0.211324865405187,  // (3.0-sqrt(3.0))/6.0\n"
	"						0.366025403784439,  // 0.5*(sqrt(3.0)-1.0)\n"
	"						-0.577350269189626,  // -1.0 + 2.0 * C.x\n"
	"						0.024390243902439); // 1.0 / 41.0\n"
	"	// First corner\n"
	"	vec2 i  = floor(v + dot(v, C.yy) );\n"
	"	vec2 x0 = v -   i + dot(i, C.xx);\n"
	"\n"
	"	// Other corners\n"
	"	vec2 i1;\n"
	"	//i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0\n"
	"	//i1.y = 1.0 - i1.x;\n"
	"	i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);\n"
	"	// x0 = x0 - 0.0 + 0.0 * C.xx ;\n"
	"	// x1 = x0 - i1 + 1.0 * C.xx ;\n"
	"	// x2 = x0 - 1.0 + 2.0 * C.xx ;\n"
	"	vec4 x12 = x0.xyxy + C.xxzz;\n"
	"	x12.xy -= i1;\n"
	"\n"
	"	// Permutations\n"
	"	i = mod289(i); // Avoid truncation effects in permutation\n"
	"	vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))\n"
	"			+ i.x + vec3(0.0, i1.x, 1.0 ));\n"
	"\n"
	"	vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);\n"
	"	m = m*m ;\n"
	"	m = m*m ;\n"
	"\n"
	"	// Gradients: 41 points uniformly over a line, mapped onto a diamond.\n"
	"	// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)\n"
	"\n"
	"	vec3 x = 2.0 * fract(p * C.www) - 1.0;\n"
	"	vec3 h = abs(x) - 0.5;\n"
	"	vec3 ox = floor(x + 0.5);\n"
	"	vec3 a0 = x - ox;\n"
	"\n"
	"	// Normalise gradients implicitly by scaling m\n"
	"	// Approximation of: m *= inversesqrt( a0*a0 + h*h );\n"
	"	m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );\n"
	"\n"
	"	// Compute final noise value at P\n"
	"	vec3 g;\n"
	"	g.x  = a0.x  * x0.x  + h.x  * x0.y;\n"
	"	g.yz = a0.yz * x12.xz + h.yz * x12.yw;\n"
	"	return 130.0 * dot(m, g);\n"
	"}\n"
	"\n"
	"// Simplex noise -- end\n"
	"float luminance(vec4 color)\n"
	"{\n"
	"	//(0.299*R + 0.587*G + 0.114*B)\n"
	"	return color.r*0.299+color.g*0.587+color.b*0.114;\n"
	"}\n"
	"\n"
	"vec2 center = vec2(1.0, direction);\n"
	"\n"
	"void main()\n"
	"{\n"
	"	vec2 uv = texCoord.xy;\n"
	"	vec2 p = uv.xy / vec2(1.0).xy;\n"
	"	if (progress == 0.0) {\n"
	"		fragColor = texture(texture0, p);\n"
	"		return;\n"
	"	} else if (progress == 1.0) {\n"
	"		fragColor = texture(texture1, p);\n"
	"		return;\n"
	"	} else {\n"
	"		float x = progress;\n"
	"		float dist = distance(center, p)- progress*exp(snoise(vec2(p.x, 0.0)));\n"
	"		float r = x - rand(vec2(p.x, 0.1));\n"
	"		float m;\n"
	"		if(above){\n"
	"			m = dist <= r && luminance(texture(texture0, p))>l_threshold ? 1.0 : (progress*progress*progress);\n"
	"		}\n"
	"		else{\n"
	"			m = dist <= r && luminance(texture(texture0, p))<l_threshold ? 1.0 : (progress*progress*progress);\n"
	"		}\n"
	"		fragColor = mix(texture(texture0, p), texture(texture1, p), m);\n"
	"	}\n"
	"}\0";