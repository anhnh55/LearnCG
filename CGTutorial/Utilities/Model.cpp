#include <Model.h>
#include <iostream>
#include <stb_image.h>
namespace HAUtilities {
    unsigned int TextureFromFile(string path, bool gamma = false);

    void Model::Draw(Shader& shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

    void Model::loadModel(string path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('\\'));

        processNode(scene->mRootNode, scene);
    }

    //assume data always at least have and start with postion data
    void Model::loadModel(float* vertexData, int dataSize, bool hasNormalData, bool hasTexCoordData, vector<string> textureData)
    {
        vector<Vertex> vertices;
        vector<Texture> textures;
        vector<unsigned int> indices;
        meshes.clear();

        int stride = 3;

        if (hasNormalData) {
            stride += 3;
        }

        if (hasTexCoordData) {
            stride += 2;
        }
        int index = 0;
        for (int i = 0; i < dataSize; i += stride) {
            index = i;
            Vertex vertex;
            vertex.Position = vec3(vertexData[index], vertexData[index + 1], vertexData[index + 2]);
            index += 3;
            if (hasNormalData) {
                vertex.Normal = vec3(vertexData[index], vertexData[index + 1], vertexData[index + 2]);
                index += 3;
            }
            else {
                vertex.Normal = vec3(0.0f, 0.0f, 0.0f);
            }

            if (hasTexCoordData) {
                vertex.TexCoords = vec2(vertexData[index], vertexData[index + 1]);
                index += 2;
            }
            else {
                vertex.TexCoords = vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        if (textureData.size() > 0) {
            
            for (int i = 0; i < textureData.size(); i += 2) {
                Texture texture;
                texture.id = TextureFromFile(textureData[i + 1]);
                texture.type = textureData[i];
                texture.fileName = textureData[i + 1].find_last_of("\\");
                textures.push_back(texture);
            }
        }

        meshes.push_back(Mesh(vertices, indices, textures));
    }    

    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // process all the node's meshes (if any)
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        vector<Vertex> vertices;
        vector<unsigned int> indices;
        vector<Texture> textures;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            // process vertex positions, normals and texture coordinates
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
                
            vertices.push_back(vertex);

            
        }
        // process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // process material
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            vector<Texture> diffuseMaps = loadMaterialTextures(material,
                aiTextureType_DIFFUSE, DIFFUSE_TEXTURE_STRING);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            vector<Texture> specularMaps = loadMaterialTextures(material,
                aiTextureType_SPECULAR, SPECULAR_TEXTURE_STRING);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(vertices, indices, textures);
    }

    unsigned int TextureFromFile(string path, bool gamma)
    {
        //string filename = string(name);
        //filename = directory + '\\' + filename;
        std::cout << "Loading texture at path: " << path << std::endl;
        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
            return 0;
        }

        return textureID;
    }

    vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
    {
        vector<Texture> textures;
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (std::strcmp(textures_loaded[j].fileName.data(), str.C_Str()) == 0)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                string path = this->directory + "\\" + string(str.C_Str());
                texture.id = TextureFromFile(path);
                texture.type = typeName;
                texture.fileName = string(str.C_Str());
                textures.push_back(texture);
                textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
            }
        }
        return textures;
    }

    void Model::SetTextures(vector<Texture> textures) {
        for (int i = 0; i < meshes.size(); ++i) {
            meshes[i].SetTextures(textures);
        }
    }
}