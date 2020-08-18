#include <Mesh.h>
#include <iostream>
namespace HAUtilities {
	using namespace std;
	Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices.resize(vertices.size());
		copy(vertices.begin(), vertices.end(), this->vertices.begin());
		
		this->indices.resize(indices.size());
		copy(indices.begin(), indices.end(), this->indices.begin());

		this->textures.resize(textures.size());
		copy(textures.begin(), textures.end(), this->textures.begin());
		VAO = VBO = EBO = 0;
		SetUpMesh();
	}

	void Mesh::Draw(Shader& shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for (int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
			 //retrieve texture number (the N in diffuse_textureN)
			string number;
			string textureType = textures[i].type;
			if (textureType == DIFFUSE_TEXTURE_STRING) {
				number = std::to_string(diffuseNr);
				++diffuseNr;
			}
				
			else if (textureType == SPECULAR_TEXTURE_STRING) {
				number = std::to_string(specularNr);
				++specularNr;
			}

			shader.SetUniform(("material." + textureType + number), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		// draw mesh
		glBindVertexArray(VAO);
		if (indices.size() > 0) {
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
		else {
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		}
		
		glBindVertexArray(0);
	}

	void Mesh::SetTextures(vector<Texture> textures)
	{
		this->textures.clear();
		this->textures.resize(textures.size());
		copy(textures.begin(), textures.end(), this->textures.begin());
	}

	void Mesh::SetUpMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		if (indices.size() > 0) {
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		}

		glEnableVertexAttribArray((GLuint)ATTRIB_LAYOUT::POSITION);
		glVertexAttribPointer((GLuint)ATTRIB_LAYOUT::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

		glEnableVertexAttribArray((GLuint)ATTRIB_LAYOUT::NORMAL);
		glVertexAttribPointer((GLuint)ATTRIB_LAYOUT::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

		glEnableVertexAttribArray((GLuint)ATTRIB_LAYOUT::TEXCOORD);
		glVertexAttribPointer((GLuint)ATTRIB_LAYOUT::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

		glBindVertexArray(0);
	}


}