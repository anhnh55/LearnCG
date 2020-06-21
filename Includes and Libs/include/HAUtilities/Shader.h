#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>

namespace HAUtilities {
    class Shader
    {
    public:
        // constructor reads and builds the shader
        Shader(const char* vertexPath, const char* fragmentPath);

        // use/activate the shader
        void Use() const;

        // utility uniform functions
        //NOTE: remember to use program before calling these functions
        void SetUniform(const std::string& name, GLboolean value) const;
        void SetUniform(const std::string& name, GLint value) const;
        void SetUniform(const std::string& name, GLfloat value) const;
        void SetUniform(const std::string& name, GLfloat value1, GLfloat value2, GLfloat value3) const;
        void SetMatrixUniform(const std::string& name, GLsizei count, GLboolean transpose, const GLfloat* value, GLuint matrixSize = 4) const;

        GLuint GetProgramID() const;
    private:
        // the program ID
        GLuint programID;
        GLuint CreateShader(GLenum shaderType, std::string source);
    };
}


