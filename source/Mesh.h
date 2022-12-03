#pragma once

#include <GL/glew.h>

class Mesh
{
private:
    GLuint m_VAO, m_VBO, m_IBO;
    GLsizei m_IndexCount;

public:
    Mesh();
    ~Mesh();

    void Create(GLfloat *vertices, GLuint vertexCount, GLuint *indices, GLuint indexCount);
    void Render();
    void Clear();
};
