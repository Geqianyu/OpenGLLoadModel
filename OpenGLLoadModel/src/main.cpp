
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

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 20.0f));
float last_x = SCREEN_WIDTH / 2.0f;
float last_y = SCREEN_WIDTH / 2.0f;
bool first_mouse = true;

// timing
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

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, 600, "LearnOpenGL", nullptr, nullptr);
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

    // Shader shader_program("asset/shader/loadModel.vert.glsl", "asset/shader/loadModel.frag.glsl");

    Shader shader_program("asset/shader/blinn-phong.vert.glsl", "asset/shader/blinn-phong.frag.glsl");
    Shader light_shader("asset/shader/lightShader.vert.glsl", "asset/shader/lightShader.frag.glsl");

    Light light(glm::vec3(20.0f, 20.0f, 20.0f), glm::vec3(50.0f, 50.0f, 50.0f));

    Model model_obj("asset/obj/nanosuit/nanosuit.obj");
    Model floor_obj("asset/obj/floor/floor.obj");

    while (!glfwWindowShouldClose(window))
    {
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        process_inport(window);

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
        model_obj.draw(shader_program);
        floor_obj.draw(shader_program);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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
