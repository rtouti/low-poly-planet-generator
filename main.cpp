#include <iostream>
#include <GL/gl3w.h>

#include "Window.h"

int main(){
    std::cout << "Multiverse" << std::endl;

    Window window("Multiverse", 800, 600);

    gl3wInit();
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glViewport(0, 0, 800, 600);

    while(window.IsOpen()){
        window.PollEvents();

        if(window.IsKeyDown(GLFW_KEY_ESCAPE))
            window.Close();

        glClearColor(0.5f, 1.0f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window.SwapBuffers();

        window.UpdateInput();
    }
}
