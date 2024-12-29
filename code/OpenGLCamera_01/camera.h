#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QVector3D>

class Camera
{
public:
    enum Movement
    {
        FORWARD = 0,
        BACKWARD,
        LEFT,
        RIGHT
    };

    Camera(float yaw = 90.0f, float pitch = 0.0f, const QVector3D& position = QVector3D(0.0f, 0.0f, 0.0f),
           const QVector3D& up = QVector3D(0.0f, 1.0f, 0.0f),
           const QVector3D& front = QVector3D(0.0f, 0.0f, 1.0f));
    ~Camera();

    QMatrix4x4 getViewMatrix();
    void processKeyboard(Movement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processMouseScroll(float yoffset);
    void updateCameraVectors();
    float getZoom() const { return m_zoom; }

private:
    QVector3D m_position;
    QVector3D m_up;
    QVector3D m_front;
    QVector3D m_right;
    QVector3D m_worldUp;

    float m_yaw;
    float m_pitch;
    float m_moveSpeed;
    float m_mouseSensitivity;
    float m_zoom;
};
#endif