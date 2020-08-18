#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include "Shader.h"
namespace HAUtilities {
    using namespace glm;
    using namespace std;
#define DIFFUSE_TEXTURE_STRING "texture_diffuse"
#define SPECULAR_TEXTURE_STRING "texture_specular"
    enum ATTRIB_LAYOUT {
        POSITION = 0,
        NORMAL = 1,
        TEXCOORD = 2
    };

    struct Vertex {
        vec3 Position;
        vec3 Normal;
        vec2 TexCoords;
    };

    struct Texture {
        unsigned int id;
        string type;
        string fileName;
    };

    class Mesh {
    public:
        // mesh data
        vector<Vertex>       vertices;
        vector<unsigned int> indices;
        vector<Texture>      textures;

        Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
        void Draw(Shader& shader);
        void SetTextures(vector<Texture> textures);
    private:
        //  render data
        unsigned int VAO, VBO, EBO;

        void SetUpMesh();
    };
}