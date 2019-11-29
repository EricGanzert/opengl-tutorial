// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

int main()
{
    // Initialize
    glewExperimental = true; // Needed for core profile
    if(!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
    }

    return 0;
}