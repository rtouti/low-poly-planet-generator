#include <iostream>
#include <GL/gl3w.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Planet.h"
#include "Shader.h"


int main(){
    std::cout << "Multiverse" << std::endl;

    Window window("Multiverse", 800, 600);

    gl3wInit();
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Camera camera;
    camera.SetProjection(70.0f, 800.0f/600.0f, 0.1f, 10000.0f);

    Planet planet(2000.0f);
    float z = 0.0;

    int frames = 0;
    double time = glfwGetTime();
    double dt = 0.0;

    while(window.IsOpen()){
        //FPS counter
        dt += glfwGetTime()-time;
        time = glfwGetTime();
        frames++;
        if(dt >= 1.0){
            std::cout << "FPS: " << frames << std::endl;
            dt -= 1.0;
            frames = 0;
        }

        window.PollEvents();

        if(window.IsKeyDown(GLFW_KEY_ESCAPE))
            window.Close();
        if(window.IsKeyPressed(GLFW_KEY_U))
            planet.update = !planet.update;

        glClearColor(0.5f, 1.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        z -= 0.001f;
        camera.Update(window);

        planet.Update(camera);

        planet.Render(camera);

        window.SwapBuffers();
        window.UpdateInput();
    }
}
