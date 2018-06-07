#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>


Camera::Camera()
: position(glm::vec3(0.0f, 0.0f, 0.0f)), projection(glm::mat4(1.0)), view(glm::mat4(1.0)), pitch(0.0f), yaw(0.0f), sensitivity(0.5f), speed(2.0f)
{

}

Camera::~Camera()
{

}

void Camera::Update(Window& window){
    if(window.IsMouseDown(GLFW_MOUSE_BUTTON_LEFT)){
        yaw += window.GetMouseDX()*sensitivity;
        pitch -= window.GetMouseDY()*sensitivity;
    }

    glm::vec3 front = glm::normalize(glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    ));
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), front));
    glm::vec3 up = glm::normalize(glm::cross(front, right));

    if(window.IsKeyDown(GLFW_KEY_W))
        position += front*speed;
    else if(window.IsKeyDown(GLFW_KEY_S))
        position -= front*speed;
    if(window.IsKeyDown(GLFW_KEY_D))
        position -= right*speed;
    else if(window.IsKeyDown(GLFW_KEY_A))
        position += right*speed;

    view = glm::lookAt(position, position+front, up);
}

glm::vec3 Camera::GetPosition(){
    return position;
}

glm::mat4 Camera::GetViewMatrix(){
    return view;
}

glm::mat4 Camera::GetProjectionMatrix(){
    return projection;
}

void Camera::SetProjection(float fov, float ratio, float near, float far){
    projection = glm::perspective(fov, ratio, near, far);
}
