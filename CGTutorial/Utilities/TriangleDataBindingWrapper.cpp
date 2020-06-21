#include <TriangleDataBindingWrapper.h>

namespace HAUtilities {
	TriangleDataBindingWrapper::TriangleDataBindingWrapper(float* dataPtr, GLsizeiptr size, GLuint positionLayout, GLuint colorLayout) 
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, size, dataPtr, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(positionLayout, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(positionLayout);
        // color attribute
        glVertexAttribPointer(colorLayout, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(colorLayout);
	}

    TriangleDataBindingWrapper::TriangleDataBindingWrapper(GLfloat* posPtr, GLsizeiptr posDataSize, 
                                                           GLfloat* colorPtr, GLsizeiptr colorDataSize, 
                                                           GLfloat* texCoordPtr, GLsizeiptr texCoordDataSize,
                                                           GLuint* indicesPtr, GLsizeiptr indicesDataSize,
                                                           GLfloat* normalPtr, GLsizeiptr normalDataSize,
                                                           GLuint positionLayout, GLuint colorLayout, GLuint texCoordLayout, GLuint normalLayout): VBO(0)
    {
        VBOs = new GLuint[VBO_SIZE];
        glGenVertexArrays(1, &VAO);
        glGenBuffers(VBO_SIZE, VBOs);
        glGenBuffers(1, &EBO);
        //bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        // position attribute
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[VBO_POS_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, posDataSize, posPtr, GL_STATIC_DRAW);
        glVertexAttribPointer(positionLayout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(positionLayout);
        
        // normal vector attribute
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[VBO_NORMAL_VEC_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, normalDataSize, normalPtr, GL_STATIC_DRAW);
        glVertexAttribPointer(normalLayout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(normalLayout);

        // color attribute
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[VBO_COLOR_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, colorDataSize, colorPtr, GL_STATIC_DRAW);
        glVertexAttribPointer(colorLayout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(colorLayout);

        //texture coord attribute
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[VBO_TEXCOORD_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, texCoordDataSize, texCoordPtr, GL_STATIC_DRAW);
        glVertexAttribPointer(texCoordLayout, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(texCoordLayout);


        //EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesDataSize, indicesPtr, GL_STATIC_DRAW);
    }

    void TriangleDataBindingWrapper::Use() {
        glBindVertexArray(VAO);
    }

    void TriangleDataBindingWrapper::Delete() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(VBO_SIZE, VBOs);
        glDeleteBuffers(1, &EBO);
    }
}