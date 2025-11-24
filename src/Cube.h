#ifndef CUBE_H
#define CUBE_H

#include <glad/glad.h>

class Cube {
public:
    unsigned int VAO, VBO;

    Cube();
    ~Cube();

    void Draw();
};

#endif
