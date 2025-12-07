#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

class Cube {
public:
    unsigned int VAO = 0, VBO = 0;

    Cube();
    ~Cube();
    void Draw() const;

private:
    // 36 vertices * (3 position + 3 normal)
    static const float kVertices[36 * 6];
};
