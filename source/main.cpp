#include <iostream>
#include <string>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

GLuint uniformModel;
GLuint uniformProjection;

bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.5f;
float triIncrement = 0.005f;
float triAngle = 0.0f;
float triSize = 0.0f;
float triMaxSize = 2.0f;

void AddShader(GLuint program, std::string& shaderCode, GLenum shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    const GLchar* sourceCode = shaderCode.c_str();
    glShaderSource(shader, 1, &sourceCode, nullptr);

    GLint result = 0;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        GLchar errorLog[1024] = {0};
        glGetShaderInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        std::cerr << "Error compiling the " << shaderType << " shader: " << errorLog << std::endl;
        return;
    }

    glAttachShader(program, shader);
}

GLuint CompileShaders()
{
    GLuint shader = glCreateProgram();
    if (!shader)
    {
        std::cerr << "Error creating shader" << std::endl;
        return 0;
    }

    std::string vertexShader = R"===(
    #version 330

    layout (location = 0) in vec3 pos;

    out vec4 vColor;

    uniform mat4 model;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * model * vec4(pos, 1.0);
        vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0);
    }
    )===";
    AddShader(shader, vertexShader, GL_VERTEX_SHADER);

    std::string fragmentShader = R"===(
    #version 330

    in vec4 vColor;

    out vec4 color;

    void main()
    {
        color = vColor;
    }
    )===";
    AddShader(shader, fragmentShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result)
    {
        GLchar errorLog[1024] = {0};
        glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        std::cerr << "Error linking program: " << errorLog << std::endl;
        return 0;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        GLchar errorLog[1024] = {0};
        glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        std::cerr << "Error validating program: " << errorLog << std::endl;
        return 0;
    }

    uniformModel = glGetUniformLocation(shader, "model");
    uniformProjection = glGetUniformLocation(shader, "projection");

    return shader;
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Couldn't start GLFW" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);
    if (!mainWindow)
    {
        std::cerr << "Couldn't create main window" << std::endl;
        glfwTerminate();
        return 1;
    }

    int bufferWidth = 0;
    int bufferHeight = 0;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Couldn't start GLEW" << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, bufferWidth, bufferHeight);

    GLfloat verticesPositions[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};

    GLuint verticesIndices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2};

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(verticesIndices), verticesIndices, GL_STATIC_DRAW);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPositions), verticesPositions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLuint shader = CompileShaders();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

    while (!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();

        // if (direction)
        // {
        //     triOffset += triIncrement;
        // }
        // else
        // {
        //     triOffset -= triIncrement;
        // }

        // if (abs(triOffset) >= triMaxoffset)
        // {
        //     direction = !direction;
        // }

        triAngle += 1.0f;
        if (triAngle >= 360)
        {
            triAngle -= 360;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, glm::radians(triAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}
