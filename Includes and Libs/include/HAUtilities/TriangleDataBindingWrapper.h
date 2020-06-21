#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers
namespace HAUtilities {
#define VBO_POS_INDEX 0
#define VBO_COLOR_INDEX 1
#define VBO_TEXCOORD_INDEX 2
#define VBO_INDICES_INDEX 3
#define VBO_NORMAL_VEC_INDEX 4
#define VBO_SIZE 5
	class TriangleDataBindingWrapper
	{
	public:
		/*
		Data has to be in following format otherwise behaviour is undefined:
		{
			x1, y1, z1, r1, g1, b1, //point 1
			x2, y2, z2, r2, g2, b2,//point 2
			.
			.
			.
			xn, yn, zn, rn, gn, bn //point n
		}
		*/
		TriangleDataBindingWrapper(float* dataPtr, GLsizeiptr dataSize, GLuint positionLayout, GLuint colorLayout);
		TriangleDataBindingWrapper(GLfloat* posPtr, GLsizeiptr posDataSize,
								   GLfloat* colorPtr = nullptr, GLsizeiptr colorDataSize = 0, 
								   GLfloat* texCoordPtr = nullptr, GLsizeiptr texCoordDataSize = 0,
								   GLuint* indicesPtr = nullptr, GLsizeiptr indicesDataSize = 0,
								   GLfloat* normalPtr = nullptr, GLsizeiptr normalDataSize = 0,
								   GLuint positionLayout = 0, GLuint colorLayout = 1, GLuint texCoordLayout = 2, GLuint normalLayout = 3);
		void Use();
		void Delete();
	private:
		GLuint VAO, VBO, EBO;
		GLuint* VBOs;
	};
}
