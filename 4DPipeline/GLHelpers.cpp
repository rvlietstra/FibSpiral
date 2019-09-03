#include "pch.h"
#include "GLHelpers.h"

namespace GLHelpers
{
	int CompileShader(GLuint &shader, int shaderType, const char* text)
	{
		shader = glCreateShader(shaderType);
		glShaderSource(shader, 1, &text, nullptr);
		glCompileShader(shader);
		GLint success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
			vector<char> error(length);
			glGetShaderInfoLog(shader, length, &length, &error[0]);
			std::cout << "Failed to compile shader: " << &error[0] << std::endl;
			glDeleteShader(shader);
			return -1;
		}

		return 0;
	}

	int CreateProgram(GLuint &program, vector<GLuint> shaders)
	{
		program = glCreateProgram();
		for (auto shader : shaders)
			glAttachShader(program, shader);

		//
		glLinkProgram(program);

		GLint success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(program);
			cout << &infoLog[0] << endl;
			return -1;
		}

		return 0;
	}
}
