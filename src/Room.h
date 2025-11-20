#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Cube.h"
#include "Shader.h"

class Room {
public:
    Cube cube;

    void Draw(Shader &shader) {
        shader.use();

        // Floor
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, -0.5f, 0));
        model = glm::scale(model, glm::vec3(10, 0.1f, 14));
        shader.setMat4("model", model);
        cube.Draw();

        // Back wall
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 2, -7));
        model = glm::scale(model, glm::vec3(10, 4, 0.1f));
        shader.setMat4("model", model);
        cube.Draw();

        // Front wall
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 2, 7));
        model = glm::scale(model, glm::vec3(10, 4, 0.1f));
        shader.setMat4("model", model);
        cube.Draw();

        // Left wall
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-5, 2, 0));
        model = glm::scale(model, glm::vec3(0.1f, 4, 14));
        shader.setMat4("model", model);
        cube.Draw();

        // Right wall
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5, 2, 0));
        model = glm::scale(model, glm::vec3(0.1f, 4, 14));
        shader.setMat4("model", model);
        cube.Draw();

        // Ceiling
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 4, 0));
        model = glm::scale(model, glm::vec3(10, 0.1f, 14));
        shader.setMat4("model", model);
        cube.Draw();
    }
};
