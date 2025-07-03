//
#include "glad/glad.h"
//
#include "GLFW/glfw3.h"
#include "Shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "stb/stb_image.h"

#define fn auto // I like this

glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
                             glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
                             glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
                             glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
                             glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

// clang-format off
float vertices[] = {
    // positions           // colors         // tex coords

    // back face
    -0.5f, -0.5f, -0.5f,   1,0,0,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   0,1,0,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0,0,1,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   1,1,0,   0.0f, 1.0f,

    // front face
    -0.5f, -0.5f,  0.5f,   1,0,1,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0,1,1,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1,1,1,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.2f,0.3f,0.4f, 0.0f, 1.0f,

    // left face
    -0.5f, -0.5f, -0.5f,   1,0,0,   0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0,1,0,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,   0,0,1,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   1,1,0,   0.0f, 1.0f,

    // right face
     0.5f, -0.5f, -0.5f,   1,0,1,   0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0,1,1,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1,1,1,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.2f,0.3f,0.4f, 0.0f, 1.0f,

    // bottom face
    -0.5f, -0.5f, -0.5f,   1,0,0,   0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,   0,1,0,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0,0,1,   1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   1,1,0,   0.0f, 1.0f,

    // top face
    -0.5f,  0.5f, -0.5f,   1,0,1,   0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0,1,1,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1,1,1,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.2f,0.3f,0.4f, 0.0f, 1.0f,
};

unsigned int indices[] = {
    0, 1, 2,  2, 3, 0,        // back
    4, 5, 6,  6, 7, 4,        // front
    8, 9,10, 10,11, 8,        // left
   12,13,14, 14,15,12,        // right
   16,17,18, 18,19,16,        // bottom
   20,21,22, 22,23,20         // top
};

// clang-format on

float deltaTime = 0.0f;
float lastFrame = 0.0f;

auto cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
auto cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

fn handleInput(GLFWwindow* window) -> void
{
    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos.y += cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos.y -= cameraSpeed;
}

fn framebuffer_size_callback(GLFWwindow* window, const int width, const int height) -> void
{
    glViewport(0, 0, width, height);
}

fn mouse_callback(GLFWwindow* window, const double xpos, const double ypos) -> void
{
    if (firstMouse)
    {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float x_offset = static_cast<float>(xpos) - lastX;
    float y_offset = lastY - static_cast<float>(ypos);
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    constexpr float sensitivity = 0.1f;
    x_offset *= sensitivity;
    y_offset *= sensitivity;

    yaw += x_offset;
    pitch += y_offset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = static_cast<float>(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
    direction.y = static_cast<float>(sin(glm::radians(pitch)));
    direction.z = static_cast<float>(sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    cameraFront = glm::normalize(direction);
}

fn main() -> int
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        return -1;
    }

    const Shader shader("src/vert.glsl", "src/frag.glsl");

    uint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;

    auto* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture\n";
    }

    stbi_image_free(data);

    uint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // VAO

    // position attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    // color attrib
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attrib
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // UNLINKING VAO
    glBindVertexArray(0);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    while (!glfwWindowShouldClose(window))
    {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        handleInput(window);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        auto model = glm::mat4(1.0f);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        glm::mat4 view;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        shader.use();
        // set uniform things
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        glBindVertexArray(VAO);

        // create box
        for (unsigned int i = 0; i < 10; i++)
        {
            auto m_model = glm::mat4(1.0f);
            m_model = glm::translate(m_model, cubePositions[i]);
            const float angle = 20.0f * static_cast<float>(i);
            m_model = glm::rotate(m_model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", m_model);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteTextures(1, &texture);
    glfwTerminate();
}
