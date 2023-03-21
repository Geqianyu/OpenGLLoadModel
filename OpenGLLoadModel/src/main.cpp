
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Log/Log.h"
#include "Shader/Shader.h"
#include "Camera/Camera.h"
#include "Model/Model.h"
#include "Light/Light.h"

void framebuffer_size_callback(GLFWwindow* _window, int _width, int _height);
void process_inport(GLFWwindow* _window);
void mouse_callback(GLFWwindow* _window, double _x_position, double _y_position);
void scroll_callback(GLFWwindow* _window, double _x_offset, double _y_offset);

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
const unsigned int SHADOW_WIDTH = 1600;
const unsigned int SHADOW_HEIGHT = 1200;

Camera camera(glm::vec3(0.0f, 10.0f, 20.0f));
float last_x = SCREEN_WIDTH / 2.0f;
float last_y = SCREEN_WIDTH / 2.0f;
bool first_mouse = true;

float delta_time = 0.0f;
float last_frame = 0.0f;

int main()
{
    Log::init();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, 600, "OpenGL Load Model", nullptr, nullptr);
    if (window == nullptr)
    {
        GQY_OPENGL_LOAD_MODEL_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        GQY_OPENGL_LOAD_MODEL_ERROR("Failed to initialize GLAD");
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader shader_program("asset/shader/blinn-phong.vert.glsl", "asset/shader/blinn-phong.frag.glsl");
    Shader light_shader("asset/shader/lightShader.vert.glsl", "asset/shader/lightShader.frag.glsl");
    Shader depth_map_shader("asset/shader/depthMap.vert.glsl", "asset/shader/depthMap.frag.glsl");

    Light light(glm::vec3(20.0f, 20.0f, 20.0f), glm::vec3(50.0f, 50.0f, 50.0f));

    Model model_obj("asset/obj/nanosuit/nanosuit.obj");
    Model floor_obj("asset/obj/floor/floor.obj");

    GLuint depth_map;
    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    GLuint depth_map_FBO;
    glGenFramebuffers(1, &depth_map_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLfloat near_plane = 20.0f, far_plane = 100.0f;

    while (!glfwWindowShouldClose(window))
    {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;
        float light_position_x = 20.0f * cosf(current_frame);
        float light_position_z = 20.0f * sinf(current_frame);
        light.set_position(glm::vec3(light_position_x, 20.0f, light_position_z));

        process_inport(window);

        glm::mat4 light_project = glm::perspective(glm::radians(60.0f), 1.0f, near_plane, far_plane);
        glm::mat4 light_view = glm::lookAt(light.get_position(), glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 light_space_matrix = light_project * light_view;
        depth_map_shader.use();
        depth_map_shader.set_matrix4("light_space_matrix", light_space_matrix);
        depth_map_shader.set_matrix4("model", glm::mat4(1.0f));
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depth_map_FBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        model_obj.draw(depth_map_shader);
        floor_obj.draw(depth_map_shader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera.get_zoom()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.get_view_matrix();
        glm::mat4 model = glm::mat4(1.0f);

        light_shader.use();
        light_shader.set_matrix4("projection", projection);
        light_shader.set_matrix4("view", view);
        light_shader.set_matrix4("model", model);
        light_shader.set_vec3("offset", light.get_position());
        light.draw(light_shader);

        shader_program.use();
        shader_program.set_vec3("light.position", light.get_position());
        shader_program.set_vec3("light.intensity", light.get_intensity());
        shader_program.set_float("light.constant", light.get_constant());
        shader_program.set_float("light.linear", light.get_linear());
        shader_program.set_float("light.quadratic", light.get_quadratic());
        shader_program.set_vec3("camera_position", camera.get_position());
        shader_program.set_matrix4("projection", projection);
        shader_program.set_matrix4("view", view);
        shader_program.set_matrix4("model", model);
        shader_program.set_matrix4("light_space_matrix", light_space_matrix);
        glActiveTexture(GL_TEXTURE0);
        shader_program.set_int("depth_map", 0);
        glBindTexture(GL_TEXTURE_2D, depth_map);
        model_obj.draw(shader_program);
        floor_obj.draw(shader_program);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteFramebuffers(1, &depth_map_FBO);
    glDeleteTextures(1, &depth_map);
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow* _window, int _width, int _height)
{
    glViewport(0, 0, _width, _height);
}

void process_inport(GLFWwindow* _window)
{
    if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(_window, true);
    }

    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.process_keyboard(Camera_move_direction::Forward, delta_time);
    }
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.process_keyboard(Camera_move_direction::Backward, delta_time);
    }
    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.process_keyboard(Camera_move_direction::Left, delta_time);
    }
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.process_keyboard(Camera_move_direction::Right, delta_time);
    }
}

void mouse_callback(GLFWwindow* _window, double _x_position, double _y_position)
{
    float xpos = static_cast<float>(_x_position);
    float ypos = static_cast<float>(_y_position);

    if (first_mouse)
    {
        last_x = xpos;
        last_y = ypos;
        first_mouse = false;
    }

    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos;

    last_x = xpos;
    last_y = ypos;

    camera.process_mouse_movement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* _window, double _x_offset, double _y_offset)
{
    camera.process_mouse_scroll(static_cast<float>(_y_offset));
}
