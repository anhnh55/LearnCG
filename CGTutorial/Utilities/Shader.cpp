#include <Shader.h>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
namespace HAUtilities {
	Shader::Shader(const char* vertexPath, const char* fragmentPath)
	{
		this->programID = 0;
		ifstream vertexFS, fragmentFS;
		stringstream vertexStrStr, fragmentStrStr;

		vertexFS.open(vertexPath);
		if (vertexFS.is_open())
		{
			vertexStrStr << vertexFS.rdbuf();
			cout << vertexStrStr.str() << endl;
		}
		else {
			cout << "FAIL TO OPEN vertex shader file" << endl;
			return;
		}
		fragmentFS.open(fragmentPath);
		if (fragmentFS.is_open())
		{
			fragmentStrStr << fragmentFS.rdbuf();
			cout << fragmentStrStr.str() << endl;
		}
		else
		{
			cout << "FAIL TO OPEN fragment shader file" << endl;
			return;
		}

		GLuint vertProg, fragProg;
		vertProg = CreateShader(GL_VERTEX_SHADER, vertexStrStr.str());
		fragProg = CreateShader(GL_FRAGMENT_SHADER, fragmentStrStr.str());

		// link shaders
		this->programID = glCreateProgram();
		glAttachShader(this->programID, vertProg);
		glAttachShader(this->programID, fragProg);
		glLinkProgram(this->programID);

		// check for linking errors
		int success;
		char infoLog[512];
		glGetProgramiv(this->programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(this->programID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			return;
		}
		cout << "SUCCESSFULLY LINK PROGRAM" << endl;
		glDeleteShader(vertProg);
		glDeleteShader(fragProg);
	}

	void Shader::Use() const
	{
		if (this->programID == 0) {
			cout << "PROGRAM INVALID" << endl;
			return;
		}
		glUseProgram(this->programID);
	}

	void Shader::SetUniform(const std::string& name, GLfloat value) const
	{
		GLuint uniformLocation = glGetUniformLocation(programID, name.c_str());
		glUniform1f(uniformLocation, value);
	}

	void Shader::SetUniform(const std::string& name, GLfloat value1, GLfloat value2, GLfloat value3) const
	{
		GLuint uniformLocation = glGetUniformLocation(programID, name.c_str());
		glUniform3f(uniformLocation, value1, value2, value3);
	}

	void Shader::SetMatrixUniform(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat* value, GLuint matrixSize) const
	{
		GLuint uniformLocation = glGetUniformLocation(programID, name.c_str());
		if (matrixSize == 4) {
			glUniformMatrix4fv(uniformLocation, count, transpose, value);
		}
		else if (matrixSize == 3) {
			glUniformMatrix3fv(uniformLocation, count, transpose, value);
		}
	}
		
	void Shader::SetUniform(const std::string& name, GLint value) const
	{
		GLuint uniformLocation = glGetUniformLocation(programID, name.c_str());
		glUniform1i(uniformLocation, value);
	}
		
	void Shader::SetUniform(const std::string& name, GLboolean value) const
	{
		GLuint uniformLocation = glGetUniformLocation(programID, name.c_str());
		glUniform1i(uniformLocation, (GLint)value);
	}


	unsigned int Shader::GetProgramID() const
	{
		return this->programID;
	}

	GLuint Shader::CreateShader(GLenum shaderType, string source)
	{
		GLuint id = glCreateShader(shaderType);
		const char* c_str = source.c_str();
		glShaderSource(id, 1, &c_str, NULL);
		glCompileShader(id);
		return id;
	}
}
