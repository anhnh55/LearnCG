#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers
namespace HAUtilities {
	class Texture2DWrapper {
	public:
		Texture2DWrapper(const char* filePath, GLenum internalFormat, GLenum format, 
						 GLenum textureUnit, GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter, bool flipVertical);
		void Use();
		GLuint GetTextureUnit();
	private:
		GLuint textureID;
		GLenum textureUnit, internalFormat, format, type;
	};
}