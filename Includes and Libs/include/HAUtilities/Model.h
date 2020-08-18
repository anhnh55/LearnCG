#pragma once
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace HAUtilities {
    class Model
    {
    public:
        Model(string path) //load using assimp
        {
            loadModel(path);
        }

        Model(float* vertexData, int dataSize, bool hasNormalData, bool hasTexCoordData, vector<string> textures) //load data from float array
        {
            loadModel(vertexData, dataSize, hasNormalData, hasTexCoordData, textures);
        }

        void LoadModel(float* vertexData, int dataSize, bool hasNormalData, bool hasTexCoordData, vector<string> textures) {
            loadModel(vertexData, dataSize, hasNormalData, hasTexCoordData, textures);
        }

        void SetTextures(vector<Texture> textures);

        void Draw(Shader& shader);
    private:
        // model data
        vector<Mesh> meshes;
        string directory;
        vector<Texture> textures_loaded;
        void loadModel(string path);
        void loadModel(float* vertexData, int dataSize, bool hasNormalData , bool hasTexCoordData, vector<string> textures);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
    };
}