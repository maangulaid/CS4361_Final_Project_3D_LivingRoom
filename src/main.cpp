#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "Furniture.h"
#include "Cube.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// -------- window size --------
const unsigned int SCR_WIDTH  = 1280;
const unsigned int SCR_HEIGHT = 720;

// callback to resize viewport
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// helper for hex color (table uses it)
glm::vec3 hexColor(unsigned int hex) {
    float r = ((hex >> 16) & 0xFF) / 255.0f;
    float g = ((hex >> 8)  & 0xFF) / 255.0f;
    float b = ( hex        & 0xFF) / 255.0f;
    return glm::vec3(r, g, b);
}

// Texture Loader
unsigned int LoadTexture(const char* path) {
    unsigned int tex;
    glGenTextures(1, &tex);

    int w, h, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &w, &h, &channels, 0);

    if (!data) {
        std::cout << "Failed to load texture: " << path << std::endl;
        return 0;
    }

    GLenum format = (channels == 4 ? GL_RGBA : GL_RGB);

    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return tex;
}

int main() {
    // --------- GLFW init ----------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Final Project - Room", nullptr, nullptr);
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
    glDisable(GL_CULL_FACE); // allows inside walls

    // --------- Shader ----------
    Shader roomShader("../shaders/textured.vert", "../shaders/textured.frag");

    // --------- Cube ----------
    Cube cube; // Has position + normals + UVs

    // Furniture context
    FurnitureContext furnitureCtx { cube.VAO, &roomShader };

    // --------- Load Textures (ONLY WALLS + FLOOR) ----------
    unsigned int wallTex  = LoadTexture("../textures/wall.jpg");
    unsigned int floorTex = LoadTexture("../textures/wood.jpg");
    unsigned int carpetTex = LoadTexture("../textures/carpet.jpg"); // optional if you have rug
    unsigned int fabricTex = LoadTexture("../textures/fabric.jpg");

    roomShader.use();
    roomShader.setInt("material.diffuse", 0); // <-- REQUIRED (texture unit 0)

    // --------- Camera (static) ----------
    glm::vec3 cameraPos    = glm::vec3(0, 1.2f, 4);
    glm::vec3 cameraTarget = glm::vec3(0, 1, 0);
    glm::vec3 cameraUp     = glm::vec3(0, 1, 0);

    // --------- Room positions ----------
    glm::vec3 floorPos   = glm::vec3(0, 0, 0);
    glm::vec3 floorScale = glm::vec3(10, 0.1f, 14);

    glm::vec3 ceilPos    = glm::vec3(0, 4, 0);
    glm::vec3 ceilScale  = glm::vec3(10, 0.1f, 14);

    glm::vec3 backPos    = glm::vec3(0, 2, -7);
    glm::vec3 backScale  = glm::vec3(10, 4, 0.1f);

    glm::vec3 frontPos   = glm::vec3(0, 2, 7);
    glm::vec3 frontScale = glm::vec3(10, 4, 0.1f);

    glm::vec3 leftPos    = glm::vec3(-5, 2, 0);
    glm::vec3 leftScale  = glm::vec3(0.1f, 4, 14);

    glm::vec3 rightPos   = glm::vec3(5, 2, 0);
    glm::vec3 rightScale = glm::vec3(0.1f, 4, 14);

    // --------- Render Loop ----------
    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        roomShader.use();

        // camera matrices
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        glm::mat4 projection =
            glm::perspective(glm::radians(45.0f),
                             (float)SCR_WIDTH / SCR_HEIGHT,
                             0.1f, 100.0f);

        roomShader.setMat4("view", view);
        roomShader.setMat4("projection", projection);

        // STATIC LIGHT
        roomShader.setVec3("viewPos", cameraPos);
        roomShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        roomShader.setVec3("dirLight.ambient",  glm::vec3(0.3f));
        roomShader.setVec3("dirLight.diffuse",  glm::vec3(0.8f));
        roomShader.setVec3("dirLight.specular", glm::vec3(1.0f));

        roomShader.setVec3("material.specular", glm::vec3(0.5f));
        roomShader.setFloat("material.shininess", 32.0f);

        // helper for drawing textured cubes
        auto DrawCube = [&](glm::vec3 pos, glm::vec3 scale, unsigned int tex)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::scale(model, scale);

            roomShader.setMat4("model", model);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, tex);

            cube.Draw();
        };

        // ---- Draw room ----
        DrawCube(floorPos, floorScale, floorTex);   // floor (wood)
        DrawCube(ceilPos, ceilScale, wallTex);      // ceiling (same as wall)
        DrawCube(backPos, backScale, wallTex);      // back wall
        DrawCube(frontPos, frontScale, wallTex);    // front wall
        DrawCube(leftPos, leftScale, wallTex);      // left wall
        DrawCube(rightPos, rightScale, wallTex);    // right wall

        // ---- Furniture (still solid colors) ----
        drawCoffeeTable(furnitureCtx);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
