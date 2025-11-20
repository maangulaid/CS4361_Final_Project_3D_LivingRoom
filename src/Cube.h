#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

class Cube {
public:
    unsigned int VAO, VBO;

    Cube();
    ~Cube();
    void Draw();
};
