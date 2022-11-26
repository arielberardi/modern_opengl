#include <iostream>
#include <string>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint uniformXMove;

bool direction = true;
float triOffset = 0.0f;
float triMaxoffset = 0.5f;
float triIncrement = 0.005f;

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

    uniform float xMove;

    void main()
    {
        gl_Position = vec4(pos.x + xMove, pos.y, pos.z, 1.0);
    }
    )===";
    AddShader(shader, vertexShader, GL_VERTEX_SHADER);

    std::string fragmentShader = R"===(
    #version 330

    out vec4 color;

    void main()
    {
        color = vec4(1.0, 0.0, 0.0, 1.0);
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

    uniformXMove = glGetUniformLocation(shader, "xMove");

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

    GLFWwindow* mainWindow = glfwCreateWindow(800, 640, "Hello World", NULL, NULL);
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

    glViewport(0, 0, bufferWidth, bufferHeight);

    GLfloat verticesPositions[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), verticesPositions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    GLuint shader = CompileShaders();

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(mainWindow))
    {
        glfwPollEvents();

        if (direction)
        {
            triOffset += triIncrement;
        }
        else
        {
            triOffset -= triIncrement;
        }

        if (abs(triOffset) >= triMaxoffset)
        {
            direction = !direction;
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glUniform1f(uniformXMove, triOffset);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    glfwDestroyWindow(mainWindow);
    glfwTerminate();
    return 0;
}
