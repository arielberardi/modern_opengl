#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat moveSpeed, GLfloat turnSpeed)
    : m_Position(position), m_WorldUp(up), m_Yaw(yaw), m_Pitch(pitch), m_MoveSpeed(moveSpeed), m_TurnSpeed(turnSpeed)
{
    m_Front = glm::vec3(0.0f, 0.0f, -1.0f);

    update();
}

Camera::~Camera()
{
}

void Camera::update()
{
    m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Front.y = sin(glm::radians(m_Pitch));
    m_Front.z = sin(glm::radians(m_Yaw)) * sin(glm::radians(m_Pitch));
    m_Front = glm::normalize(m_Front);

    m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

void Camera::keyControl(const std::array<bool, 1024>& keys, GLfloat deltaTime)
{
    GLfloat velocity = m_MoveSpeed * deltaTime;

    if (keys[GLFW_KEY_W])
    {
        m_Position += m_Front * velocity;
    }
    if (keys[GLFW_KEY_S])
    {
        m_Position -= m_Front * velocity;
    }
    if (keys[GLFW_KEY_D])
    {
        m_Position += m_Right * velocity;
    }
    if (keys[GLFW_KEY_A])
    {
        m_Position -= m_Right * velocity;
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= m_TurnSpeed;
    yChange *= m_TurnSpeed;

    m_Yaw += xChange;
    m_Pitch += yChange;

    if (m_Pitch > 89.0f)
    {
        m_Pitch = 89.0f;
    }

    if (m_Pitch < -89.0f)
    {
        m_Pitch = -89.0f;
    }

    update();
}

glm::mat4 Camera::calculateViewMatrix()
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}
