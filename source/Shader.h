#pragma once

#include <GL/glew.h>

class Shader
{
private:
    GLuint shaderID, uniformProjection, uniformModel, uniformView;

    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

public:
    Shader();
    ~Shader();

    void CreateFromString(std::string& vertexCode, std::string& fragmentCode);
    void CreateFromFiles(std::string& vertexLocation, std::string& fragmentLocation);

    std::string ReadFile(std::string& fileLocation);

    GLuint GetProjectionLocation();
    GLuint GetModelLocation();
    GLuint GetViewLocation();

    void UseShader();
    void ClearShader();
};
