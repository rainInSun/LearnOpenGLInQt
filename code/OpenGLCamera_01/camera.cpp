#include "camera.h"
#include <QtMath>
#include <QDebug>

Camera::Camera(float yaw, float pitch, const QVector3D& position, const QVector3D& up, const QVector3D& front)
    : m_position(position), m_up(up), m_front(front), m_yaw(yaw), m_pitch(pitch),
      m_moveSpeed(2.5f), m_mouseSensitivity(0.05f), m_zoom(45.0f), m_worldUp(QVector3D(0.0, 1.0, 0.0))
{
    updateCameraVectors();
}

Camera::~Camera()
{
}

QMatrix4x4 Camera::getViewMatrix()
{
    QMatrix4x4 view;
    view.lookAt(m_position, m_position + m_front, m_up);
    return view;
}

void Camera::processKeyboard(Movement direction, float deltaTime)
{
    float velocity = m_moveSpeed * deltaTime;
    if (direction == FORWARD)
        m_position += m_front * velocity;
    if (direction == BACKWARD)
        m_position -= m_front * velocity;
    if (direction == LEFT)
        m_position -= m_right * velocity;
    if (direction == RIGHT)
        m_position += m_right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        else if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
    m_zoom -= yoffset;
    if (m_zoom < 1.0f)
        m_zoom = 1.0f;
    else if (m_zoom > 45.0f)
        m_zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    QVector3D front;
    front.setX(qCos(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
    front.setY(qSin(qDegreesToRadians(m_pitch)));
    front.setZ(qSin(qDegreesToRadians(m_yaw)) * qCos(qDegreesToRadians(m_pitch)));
    m_front = front.normalized();
    m_right = QVector3D::crossProduct(m_front, m_worldUp).normalized();
    m_up = QVector3D::crossProduct(m_right, m_front).normalized();
}