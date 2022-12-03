#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
private:
    GLFWwindow* mainWindow;

    GLint width, height;
    GLint bufferWidth, bufferHeight;

public:
    Window(GLint windowWidth, GLint windowHeight);
    ~Window();

    int Initialise();

    GLint getBufferWidth();
    GLint getBufferHeight();

    bool getShouldClose();

    void swapBuffers();
};
