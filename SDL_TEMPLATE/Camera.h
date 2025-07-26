#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <string>
#include <glad/glad.h>

class GameWindow;

class Camera {
public:
    constexpr static float FOV_MAX = 100.0f;
    constexpr static float SPRINT_MULTIPLIER = 3.0f;
    constexpr static float FOV_SPEED_MULTIPLIER = 5.0f;

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float fov = 45.0f;

    Camera(glm::vec3 startPosition, glm::vec3 startUp,
        float startYaw, float startPitch);

    glm::mat4 getViewMatrix() const;

    void processKeyboard(SDL_Event& event, GameWindow* window);

    void processMouseMotion(SDL_Event& event);

    void setViewToShader(GLuint shaderID, const std::string& uniformName) const;

    void update();

    float getFOV() const;
private:
    void updateCameraVectors();
};