
#include "Camera.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

Camera::Camera(const glm::vec3& _position, const glm::vec3& _up, const float& _yaw, const float& _pitch) : m_position(_position), m_world_up(_up), m_yaw(_yaw), m_pitch(_pitch)
{
    update_camera_vectors();
}

Camera::Camera(const float& _pos_x, const float& _pos_y, const float& _pos_z, const float& _up_x, const float& _up_y, const float& _up_z, const float& _yaw, const float& _pitch) : m_position(glm::vec3(_pos_x, _pos_y, _pos_z)), m_world_up(glm::vec3(_up_x, _up_y, _up_z)), m_yaw(_yaw), m_pitch(_pitch)
{
    update_camera_vectors();
}