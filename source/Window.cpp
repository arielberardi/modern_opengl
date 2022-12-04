#include <iostream>
#include <array>

#include "Window.h"

Window::Window(GLint windowWidth, GLint windowHeight)
{
    width = windowWidth;
    height = windowHeight;

    keyboardKeys.fill(false);
    mouseLastPosition.fill(0.0f);
    mouseChange.fill(0.0f);
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

int Window::Initialise()
{
    if (!glfwInit())
    {
        std::cout << "Error Initialising GLFW" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
    if (!mainWindow)
    {
        std::cout << "Error creating GLFW window!" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    glfwMakeContextCurrent(mainWindow);

    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMousePosition);
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        std::cout << "Error: " << glewGetErrorString(error) << std::endl;
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, bufferWidth, bufferHeight);

    glfwSetWindowUserPointer(mainWindow, this);
    return 0;
}

GLint Window::getBufferWidth()
{
    return bufferWidth;
}

GLint Window::getBufferHeight()
{
    return bufferHeight;
}

bool Window::getShouldClose()
{
    return glfwWindowShouldClose(mainWindow);
}

void Window::swapBuffers()
{
    glfwSwapBuffers(mainWindow);
}

std::array<bool, 1024> Window::getKeys()
{
    return keyboardKeys;
}

GLfloat Window::getXChange()
{
    return getMouseChange(0);
}

GLfloat Window::getYChange()
{
    return getMouseChange(1);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* myWindowClass = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        myWindowClass->keyboardKeys[key] = action == GLFW_PRESS;
    }
}

void Window::handleMousePosition(GLFWwindow* window, double x, double y)
{
    Window* myWindowClass = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (!myWindowClass->mouseFocused)
    {
        myWindowClass->mouseLastPosition[0] = x;
        myWindowClass->mouseLastPosition[1] = y;
        myWindowClass->mouseFocused = true;
    }

    myWindowClass->mouseChange[0] = x - myWindowClass->mouseLastPosition[0];
    myWindowClass->mouseChange[1] = myWindowClass->mouseLastPosition[1] - y;  // Avoid inverted movement

    myWindowClass->mouseLastPosition[0] = x;
    myWindowClass->mouseLastPosition[1] = y;
}

GLfloat Window::getMouseChange(const int coord)
{
    GLfloat tempChange = mouseChange[coord];
    mouseChange[coord] = 0.0f;
    return tempChange;
}
