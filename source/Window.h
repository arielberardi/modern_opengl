#pragma once

#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
private:
    GLFWwindow* mainWindow;

    GLint width, height;
    GLint bufferWidth, bufferHeight;

    std::array<bool, 1024> keyboardKeys;
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);

    std::array<GLfloat, 2> mouseLastPosition;
    std::array<GLfloat, 2> mouseChange;
    bool mouseFocused;
    static void handleMousePosition(GLFWwindow* window, double x, double y);

    GLfloat getMouseChange(int coord);

public:
    Window(GLint windowWidth, GLint windowHeight);
    ~Window();

    int Initialise();

    GLint getBufferWidth();
    GLint getBufferHeight();

    bool getShouldClose();

    void swapBuffers();

    std::array<bool, 1024> getKeys();  // TODO: Improve this with design patterns

    GLfloat getXChange();
    GLfloat getYChange();
};
