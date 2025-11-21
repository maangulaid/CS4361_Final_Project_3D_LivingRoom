#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"

// -------- window size --------
const unsigned int SCR_WIDTH  = 1280;
const unsigned int SCR_HEIGHT = 720;

// callback to resize viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// simple helper: 0xRRGGBB -> glm::vec3
glm::vec3 hexColor(unsigned int hex) {
    float r = ((hex >> 16) & 0xFF) / 255.0f;
    float g = ((hex >> 8)  & 0xFF) / 255.0f;
    float b = ( hex        & 0xFF) / 255.0f;
    return glm::vec3(r, g, b);
}

// simple cube vertices (positions only), centered at origin, size 1
float cubeVertices[] = {
    // back face
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    // front face
    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    // left face
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    // right face
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    // bottom face
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    // top face
    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
};

int main()
{
    // --------- GLFW init ----------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Final Project - Room", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // --------- GLAD init ----------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);   // see inside faces

    // --------- Shader ----------
    Shader roomShader("../shaders/room.vert", "../shaders/room.frag");

    // --------- Cube VAO/VBO ----------
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // --------- Room dimensions ----------
    // room center at origin
    // width  = 10 (x: -5 to +5)
    // length = 14 (z: -7 to +7)
    // height = 4  (y:  0 to +4 for walls)

    // camera inside room at (0, 1, 3)
    glm::vec3 cameraPos    = glm::vec3(0.0f, 1.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraUp     = glm::vec3(0.0f, 1.0f, 0.0f);

    // floor at y = 0
    glm::vec3 floorPos   = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 floorScale = glm::vec3(10.0f, 0.1f, 14.0f);

    // ceiling at y = 4
    glm::vec3 ceilPos    = glm::vec3(0.0f, 4.0f, 0.0f);
    glm::vec3 ceilScale  = glm::vec3(10.0f, 0.1f, 14.0f);

    // BACK WALL (-Z)
    glm::vec3 backPos    = glm::vec3(0.0f, 2.0f, -7.0f);
    glm::vec3 backScale  = glm::vec3(10.0f, 4.0f, 0.1f);

    // FRONT WALL (+Z)
    glm::vec3 frontPos   = glm::vec3(0.0f, 2.0f,  7.0f);
    glm::vec3 frontScale = glm::vec3(10.0f, 4.0f, 0.1f);

    // LEFT WALL (-X)
    glm::vec3 leftPos    = glm::vec3(-5.0f, 2.0f, 0.0f);
    glm::vec3 leftScale  = glm::vec3(0.1f, 4.0f, 14.0f);

    // RIGHT WALL (+X)
    glm::vec3 rightPos   = glm::vec3( 5.0f, 2.0f, 0.0f);
    glm::vec3 rightScale = glm::vec3(0.1f, 4.0f, 14.0f);



    

    // --------- Colors (hex) ----------
    // You can change these hex values to anything you want (#RRGGBB)

    // if any of yall want to change the color scheme, here are the hex codes:
    glm::vec3 floorColor   = hexColor(0xFF0000); 
    glm::vec3 ceilColor    = hexColor(0xE0E0E0); 
    glm::vec3 backColor    = hexColor(0x9999FF); 
    glm::vec3 frontColor   = hexColor(0x9999FF);
    glm::vec3 sideWallColor= hexColor(0xADD8E6); 




    // --------- render loop ----------
    while (!glfwWindowShouldClose(window))
    {
        // input (press ESC to close)
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // clear
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        roomShader.use();

        // view & projection
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f, 100.0f
        );

        roomShader.setMat4("view", view);
        roomShader.setMat4("projection", projection);

        glBindVertexArray(cubeVAO);

        // --- floor ---
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, floorPos);
            model = glm::scale(model, floorScale);
            roomShader.setMat4("model", model);
            roomShader.setVec3("objectColor", floorColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // --- ceiling ---
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, ceilPos);
            model = glm::scale(model, ceilScale);
            roomShader.setMat4("model", model);
            roomShader.setVec3("objectColor", ceilColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // --- back wall (-Z) ---
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, backPos);
            model = glm::scale(model, backScale);
            roomShader.setMat4("model", model);
            roomShader.setVec3("objectColor", backColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // --- front wall (+Z) ---
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, frontPos);
            model = glm::scale(model, frontScale);
            roomShader.setMat4("model", model);
            roomShader.setVec3("objectColor", frontColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // --- left wall (-X) ---
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, leftPos);
            model = glm::scale(model, leftScale);
            roomShader.setMat4("model", model);
            roomShader.setVec3("objectColor", sideWallColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // --- right wall (+X) ---
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, rightPos);
            model = glm::scale(model, rightScale);
            roomShader.setMat4("model", model);
            roomShader.setVec3("objectColor", sideWallColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // swap buffers + poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // clean up
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);

    glfwTerminate();
    return 0;
}
