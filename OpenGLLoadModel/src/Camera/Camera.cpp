
#include "Camera.h"

Camera::Camera(const glm::vec3& _position, const glm::vec3& _up, const float& _yaw, const float& _pitch) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movement_speed(SPEED), m_mouse_sensitivity(SENSITIVITY), m_zoom(ZOOM)
{
    m_position = _position;
    m_world_up = _up;
    m_yaw = _yaw;
    m_pitch = _pitch;
    update_camera_vectors();
}

Camera::Camera(const float& _pos_x, const float& _pos_y, const float& _pos_z, const float& _up_x, const float& _up_y, const float& _up_z, const float& _yaw, const float& _pitch) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movement_speed(SPEED), m_mouse_sensitivity(SENSITIVITY), m_zoom(ZOOM)
{
    m_position = glm::vec3(_pos_x, _pos_y, _pos_z);
    m_world_up = glm::vec3(_up_x, _up_y, _up_z);
    m_yaw = _yaw;
    m_pitch = _pitch;
    update_camera_vectors();
}