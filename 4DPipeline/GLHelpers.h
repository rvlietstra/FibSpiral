#pragma once
#include <GL/glew.h>
#include <iostream>
#include <vector>

using namespace std;

namespace GLHelpers
{
	int CompileShader(GLuint &shader, int shaderType, const char* text);
	int CreateProgram(GLuint &program, vector<GLuint> shaders);
};

