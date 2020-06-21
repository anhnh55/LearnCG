#include <Texture2DWrapper.h>
#include <iostream>
#include <stb_image.h>

namespace HAUtilities {
    Texture2DWrapper::Texture2DWrapper(const char* filePath, GLenum internalFormat, GLenum format, GLenum textureUnit, 
                                       GLenum wrapS, GLenum wrapT, GLenum minFilter, GLenum magFilter, bool flipVertical)
    {
        stbi_set_flip_vertically_on_load(flipVertical);
        this->textureUnit = textureUnit;
        glGenTextures(1, &textureID);
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
        unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }

    void Texture2DWrapper::Use() {
        glActiveTexture(textureUnit);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    GLuint Texture2DWrapper::GetTextureUnit() {
        return textureUnit;
    }
}

