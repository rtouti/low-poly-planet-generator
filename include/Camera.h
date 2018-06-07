#ifndef CAMERA_H
#define CAMERA_H

#include <glm/mat4x4.hpp>

#include "Shader.h"
#include "Window.h"

class Camera
{
    public:
        Camera();
        virtual ~Camera();
        void Update(Window& window);
        glm::vec3 GetPosition();
        glm::mat4 GetViewMatrix();
        glm::mat4 GetProjectionMatrix();
        void SetProjection(float fov, float ratio, float near, float far);
    private:
        glm::vec3 position;
        glm::mat4 projection, view;
        float pitch, yaw, sensitivity, speed;
};

#endif // CAMERA_H
