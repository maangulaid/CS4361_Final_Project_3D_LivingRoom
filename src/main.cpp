#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include "shader.h"
#include "Furniture.h"   // your drawCoffeeTable / drawTVStand / drawSofa using objectColor

// ------------ window ------------
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// ------------ camera state ------------
glm::vec3 camPos = glm::vec3(1.5f, 1.4f, 6.0f);
glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yawDeg = -90.0f;
float pitchDeg = 0.0f;
float lastX = SCR_WIDTH * 0.5f;
float lastY = SCR_HEIGHT * 0.5f;
bool  firstMouse = true;
float moveSpeed = 2.5f;   // units/sec
float mouseSens = 0.08f;  // degrees/pixel

bool flashlightOn = true;
bool fogOn = false;
bool nightMode = false;   // dims ambient

// for input debounce
bool lastF = false, lastG = false, lastN = false;


// ------------ callbacks ------------
void framebuffer_size_callback(GLFWwindow*, int w, int h) {
    glViewport(0, 0, w, h);
}

void mouse_callback(GLFWwindow*, double xpos, double ypos) {
    if (firstMouse) { lastX = (float)xpos; lastY = (float)ypos; firstMouse = false; }
    float xoffset = float(xpos) - lastX;
    float yoffset = lastY - float(ypos);  // reversed since y-coordinates go from bottom to top
    lastX = (float)xpos;
    lastY = (float)ypos;

    yawDeg += xoffset * mouseSens;
    pitchDeg += yoffset * mouseSens;
    if (pitchDeg > 89.0f)  pitchDeg = 89.0f;
    if (pitchDeg < -89.0f) pitchDeg = -89.0f;

    glm::vec3 f;
    f.x = cos(glm::radians(yawDeg)) * cos(glm::radians(pitchDeg));
    f.y = sin(glm::radians(pitchDeg));
    f.z = sin(glm::radians(yawDeg)) * cos(glm::radians(pitchDeg));
    camFront = glm::normalize(f);
}

void processInput(GLFWwindow* window, float dt) {
    float v = moveSpeed * dt;
    glm::vec3 right = glm::normalize(glm::cross(camFront, camUp));
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camPos += v * camFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camPos -= v * camFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camPos -= v * right;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camPos += v * right;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) camPos.y -= v;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) camPos.y += v;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    // clamp position to room bounds
        // --- keep camera inside the room box ---
    const float minX = -4.5f;
    const float maxX =  4.5f;
    const float minZ = -6.5f;
    const float maxZ =  6.5f;
    const float minY =  0.2f;  // a bit above floor
    const float maxY =  3.5f;  // a bit below ceiling

    camPos.x = std::clamp(camPos.x, minX, maxX);
    camPos.z = std::clamp(camPos.z, minZ, maxZ);
}

// 0xRRGGBB -> glm::vec3
glm::vec3 hexColor(unsigned int hex) {
    float r = ((hex >> 16) & 0xFF) / 255.0f;
    float g = ((hex >> 8) & 0xFF) / 255.0f;
    float b = ((hex) & 0xFF) / 255.0f;
    return glm::vec3(r, g, b);
}

// Simple cube (pos+normal, 36 verts). Keep the same you had before.
static float cubeVertices[] = {
    // back (-Z)
    -0.5f,-0.5f,-0.5f,  0,0,-1,
     0.5f,-0.5f,-0.5f,  0,0,-1,
     0.5f, 0.5f,-0.5f,  0,0,-1,
     0.5f, 0.5f,-0.5f,  0,0,-1,
    -0.5f, 0.5f,-0.5f,  0,0,-1,
    -0.5f,-0.5f,-0.5f,  0,0,-1,
    // front (+Z)
    -0.5f,-0.5f, 0.5f,  0,0,1,
     0.5f,-0.5f, 0.5f,  0,0,1,
     0.5f, 0.5f, 0.5f,  0,0,1,
     0.5f, 0.5f, 0.5f,  0,0,1,
    -0.5f, 0.5f, 0.5f,  0,0,1,
    -0.5f,-0.5f, 0.5f,  0,0,1,
    // left (-X)
    -0.5f, 0.5f, 0.5f, -1,0,0,
    -0.5f, 0.5f,-0.5f, -1,0,0,
    -0.5f,-0.5f,-0.5f, -1,0,0,
    -0.5f,-0.5f,-0.5f, -1,0,0,
    -0.5f,-0.5f, 0.5f, -1,0,0,
    -0.5f, 0.5f, 0.5f, -1,0,0,
    // right (+X)
     0.5f, 0.5f, 0.5f,  1,0,0,
     0.5f, 0.5f,-0.5f,  1,0,0,
     0.5f,-0.5f,-0.5f,  1,0,0,
     0.5f,-0.5f,-0.5f,  1,0,0,
     0.5f,-0.5f, 0.5f,  1,0,0,
     0.5f, 0.5f, 0.5f,  1,0,0,
     // bottom (-Y)
     -0.5f,-0.5f,-0.5f,  0,-1,0,
      0.5f,-0.5f,-0.5f,  0,-1,0,
      0.5f,-0.5f, 0.5f,  0,-1,0,
      0.5f,-0.5f, 0.5f,  0,-1,0,
     -0.5f,-0.5f, 0.5f,  0,-1,0,
     -0.5f,-0.5f,-0.5f,  0,-1,0,
     // top (+Y)
     -0.5f, 0.5f,-0.5f,  0,1,0,
      0.5f, 0.5f,-0.5f,  0,1,0,
      0.5f, 0.5f, 0.5f,  0,1,0,
      0.5f, 0.5f, 0.5f,  0,1,0,
     -0.5f, 0.5f, 0.5f,  0,1,0,
     -0.5f, 0.5f,-0.5f,  0,1,0,
};

int main() {
    // --- init window ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Phase 5 - Light & Camera", nullptr, nullptr);
    if (!window) { std::cerr << "GLFW create window failed\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // mouse-look

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD init failed\n"; return -1;
    }
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // --- shader ---
    Shader solidShader("C:/Users/User/OneDrive/Documents/computer graphics/CS4361_Final_Project_3D_LivingRoom-main/src/room.vert",
        "C:/Users/User/OneDrive/Documents/computer graphics/CS4361_Final_Project_3D_LivingRoom-main/src/room.frag"); // <- put your real paths

    // --- cube VAO/VBO ---
    unsigned int cubeVAO = 0, cubeVBO = 0;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    // pos
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // furniture context
    FurnitureContext ctx{ cubeVAO, &solidShader };

    // room blocks (positions + scales)
    glm::vec3 floorPos = { 0.0f, 0.0f,  0.0f };  glm::vec3 floorScale = { 10.0f, 0.1f, 14.0f };
    glm::vec3 ceilPos = { 0.0f, 4.0f,  0.0f };  glm::vec3 ceilScale = { 10.0f, 0.1f, 14.0f };
    glm::vec3 backPos = { 0.0f, 2.0f, -7.0f };  glm::vec3 backScale = { 10.0f, 4.0f,  0.1f };
    glm::vec3 frontPos = { 0.0f, 2.0f,  7.0f };  glm::vec3 frontScale = { 10.0f, 4.0f,  0.1f };
    glm::vec3 leftPos = { -5.0f,2.0f,  0.0f };  glm::vec3 leftScale = { 0.1f,  4.0f, 14.0f };
    glm::vec3 rightPos = { 5.0f,2.0f,  0.0f };  glm::vec3 rightScale = { 0.1f,  4.0f, 14.0f };

    // colors
    glm::vec3 floorCol = hexColor(0x6B8E23); // olive-ish
    glm::vec3 ceilCol = hexColor(0xE0E0E0);
    glm::vec3 backCol = hexColor(0xC0D6FF);
    glm::vec3 frontCol = hexColor(0xC0D6FF);
    glm::vec3 sideCol = hexColor(0xADD8E6);

    // light
    glm::vec3 lightPos(0.0f, 3.0f, 2.5f);
    glm::vec3 lightColor(1.0f);


    glm::vec3 lightPos0 = glm::vec3(0.0f, 3.0f, 2.5f);  // ceiling-ish
    glm::vec3 lightCol0 = glm::vec3(1.0f);

    glm::vec3 lightPos1 = glm::vec3(0.0f, 1.0f, -2.2f);  // near TV stand
    glm::vec3 lightCol1 = glm::vec3(1.0f, 0.95f, 0.80f); // warm

    // time for smooth movement
    float lastTime = (float)glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        float now = (float)glfwGetTime();
        float dt = now - lastTime;
        lastTime = now;
        processInput(window, dt);


        // --- toggles (F flashlight, G fog, N night) ---
        bool F = glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS;
        bool G = glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS;
        bool N = glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS;

        if (F && !lastF) flashlightOn = !flashlightOn;
        if (G && !lastG) fogOn = !fogOn;
        if (N && !lastN) nightMode = !nightMode;

        lastF = F; lastG = G; lastN = N;



        glClearColor(0.08f, 0.08f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // camera matrices
        glm::mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

       /* solidShader.use();
        solidShader.setMat4("view", view);
        solidShader.setMat4("projection", proj);
        solidShader.setVec3("lightPos", lightPos);
        solidShader.setVec3("lightColor", lightColor);
        solidShader.setVec3("viewPos", camPos);*/

        solidShader.use();
        solidShader.setMat4("view", view);
        solidShader.setMat4("projection", proj);
        solidShader.setVec3("viewPos", camPos);

        // point lights
        solidShader.setVec3("lightPos0", lightPos0);
        solidShader.setVec3("lightColor0", lightCol0);
        solidShader.setVec3("lightPos1", lightPos1);
        solidShader.setVec3("lightColor1", lightCol1);

        // flashlight (camera-mounted)
        solidShader.setInt("useFlashlight", flashlightOn ? 1 : 0);
        solidShader.setVec3("flashDir", glm::normalize(camFront));
        // inner/outer cone angles
        float innerDeg = 15.0f;
        float outerDeg = 22.0f;
        solidShader.setFloat("flashCutoff", cos(glm::radians(innerDeg)));
        solidShader.setFloat("flashOuterCutoff", cos(glm::radians(outerDeg)));
        solidShader.setVec3("flashColor", glm::vec3(1.0f, 0.98f, 0.9f));

        // ambient scaling (night mode)
        solidShader.setFloat("ambientScale", nightMode ? 0.05f : 0.15f);

        // fog
        solidShader.setInt("useFog", fogOn ? 1 : 0);
        solidShader.setVec3("fogColor", glm::vec3(0.12f, 0.12f, 0.14f));
        solidShader.setFloat("fogDensity", 0.03f);


        glBindVertexArray(cubeVAO);

        auto drawBlock = [&](glm::vec3 pos, glm::vec3 scale, glm::vec3 col) {
            glm::mat4 M(1.0f);
            M = glm::translate(M, pos);
            M = glm::scale(M, scale);
            solidShader.setMat4("model", M);
            solidShader.setVec3("objectColor", col);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            };

        // room
        drawBlock(floorPos, floorScale, floorCol);
        drawBlock(ceilPos, ceilScale, ceilCol);
        drawBlock(backPos, backScale, backCol);
        drawBlock(frontPos, frontScale, frontCol);
        drawBlock(leftPos, leftScale, sideCol);
        drawBlock(rightPos, rightScale, sideCol);

        // furniture
        drawCoffeeTable(ctx, glm::vec3(0.8f, 0.0f, -1.2f), 0.0f, glm::vec3(1.0f));
        drawTVStand(ctx, glm::vec3(0.0f, 0.50f, -4.70f));
        drawSofa(ctx, glm::vec3(3.0f, 0.0f, -1.2f), 270.0f);

        // draw a tiny lamp cube at lightPos so you can see it
        {
            glm::mat4 M(1.0f);
            M = glm::translate(M, lightPos);
            M = glm::scale(M, glm::vec3(0.12f));
            solidShader.setMat4("model", M);
            solidShader.setVec3("objectColor", hexColor(0xFFF2B2)); // pale yellow
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glfwTerminate();
    return 0;
}
