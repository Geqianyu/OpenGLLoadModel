
#ifndef GQY_CAMERA_H
#define GQY_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

enum class Camera_move_direction
{
    Forward = 0,
    Backward,
    Left,
    Right
};

class Camera
{
public:
    Camera(const glm::vec3& _position = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& _up = glm::vec3(0.0f, 1.0f, 0.0f), const float& _yaw = YAW, const float& _pitch = PITCH);
    Camera(const float& _pos_x, const float& _pos_y, const float& _pos_z, const float& _up_x, const float& _up_y, const float& _up_z, const float& _yaw, const float& _pitch);
    ~Camera() = default;

    inline glm::mat4 get_view_matrix() const
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    inline void process_keyboard(const Camera_move_direction& _direction, const float& _delta_time)
    {
        float velocity = m_movement_speed * _delta_time;
        if (_direction == Camera_move_direction::Forward)
            m_position += m_front * velocity;
        if (_direction == Camera_move_direction::Backward)
            m_position -= m_front * velocity;
        if (_direction == Camera_move_direction::Left)
            m_position -= m_right * velocity;
        if (_direction == Camera_move_direction::Right)
            m_position += m_right * velocity;
    }

    inline void process_mouse_movement(float _xoffset, float _yoffset, const GLboolean& constrain_pitch = true)
    {
        _xoffset *= m_mouse_sensitivity;
        _yoffset *= m_mouse_sensitivity;

        m_yaw += _xoffset;
        m_pitch += _yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrain_pitch)
        {
            if (m_pitch > 89.0f)
            {
                m_pitch = 89.0f;
            }
            if (m_pitch < -89.0f)
            {
                m_pitch = -89.0f;
            }
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        update_camera_vectors();
    }

    inline void process_mouse_scroll(const float& _yoffset)
    {
        m_zoom -= _yoffset;
        if (m_zoom < 1.0f)
        {
            m_zoom = 1.0f;
        }
        if (m_zoom > 45.0f)
        {
            m_zoom = 45.0f;
        }
    }

    inline float get_zoom() const
    {
        return m_zoom;
    }

private:
    inline void update_camera_vectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        m_right = glm::normalize(glm::cross(m_front, m_world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

private:
    // camera Attributes
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_world_up;
    // euler Angles
    float m_yaw;
    float m_pitch;
    // camera options
    float m_movement_speed;
    float m_mouse_sensitivity;
    float m_zoom;
};

#endif // !GQY_CAMERA_H
