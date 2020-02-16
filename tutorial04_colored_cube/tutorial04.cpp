#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <common/shader.hpp>

int main ()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and set it's OpenGL context
    window = glfwCreateWindow(1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it is closer to the camera than the former one
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("../tutorial04_colored_cube/TransformVertexShader.vertexshader",
        "../tutorial04_colored_cube/ColorFragmentShader.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    // ProjectionMatrix : 45 deg field of view, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(4,3,-5), // Camera is here in world space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0) // Head is up
    );

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model;

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2 triangles, and 13*3 vertices
    static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
    };

    // One color for each vertex. They were generated randomly.
    static GLfloat g_color_buffer_data[3 * 3 * 12];

    std::time_t timePoint;
    std::asctime(std::localtime(&timePoint));
    srand(timePoint);
    for (int i=0; i<3*3*12; i++)
    {
        g_color_buffer_data[i] = static_cast<float>(rand() % 100) / 100.0f;
    }

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    // Exercise 1: draw the triangle in addition to the cube

    GLuint TriangleVertexArrayID;
    glGenVertexArrays(1, &TriangleVertexArrayID);
    glBindVertexArray(TriangleVertexArrayID);

    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 TriangleModel = glm::translate(glm::mat4(), glm::vec3(2.0f, 2.0f, 0.0f));
    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 TriangleMVP = Projection * View * TriangleModel;

    static const GLfloat g_triangle_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    static const GLfloat g_triangle_color_buffer_data[] = {
        1.0f, 0.0f, 0.0f,
        0.8f, 0.2f, 0.0f,
        0.6f, 0.4f, 0.0f,
    };

    GLuint triangleVertexbuffer;
    glGenBuffers(1, &triangleVertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_triangle_vertex_buffer_data),
        g_triangle_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint triangleColorBuffer;
    glGenBuffers(1, &triangleColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, triangleColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_triangle_color_buffer_data),
        g_triangle_color_buffer_data, GL_STATIC_DRAW);

    do {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &TriangleMVP[0][0]);

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, triangleVertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, triangleColorBuffer);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // -------------- CUBE --------------------

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(
            0,              // attribute. No particular reason for 0, but must match the layout in the shader
            3,              // size
            GL_FLOAT,       // type
            GL_FALSE,       // normalized?
            0,              // stride
            (void*)0        // array buffer offset
        );

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indicates starting at 0 -> 12 triangles

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        // Check if the ESC key was pressed or the window was closed
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &colorBuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}