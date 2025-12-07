#include "Cube.h"
#include "stb_image.h"

// positions (x,y,z) and face normals (nx,ny,nz)
const float Cube::kVertices[36 * 6] = {
    // --- Back face (z = -0.5), normal (0,0,-1)
    -0.5f,-0.5f,-0.5f,   0.f, 0.f,-1.f,
     0.5f,-0.5f,-0.5f,   0.f, 0.f,-1.f,
     0.5f, 0.5f,-0.5f,   0.f, 0.f,-1.f,
     0.5f, 0.5f,-0.5f,   0.f, 0.f,-1.f,
    -0.5f, 0.5f,-0.5f,   0.f, 0.f,-1.f,
    -0.5f,-0.5f,-0.5f,   0.f, 0.f,-1.f,

    // --- Front face (z = +0.5), normal (0,0,1)
    -0.5f,-0.5f, 0.5f,   0.f, 0.f, 1.f,
     0.5f,-0.5f, 0.5f,   0.f, 0.f, 1.f,
     0.5f, 0.5f, 0.5f,   0.f, 0.f, 1.f,
     0.5f, 0.5f, 0.5f,   0.f, 0.f, 1.f,
    -0.5f, 0.5f, 0.5f,   0.f, 0.f, 1.f,
    -0.5f,-0.5f, 0.5f,   0.f, 0.f, 1.f,

    // --- Left face (x = -0.5), normal (-1,0,0)
    -0.5f,-0.5f,-0.5f,  -1.f, 0.f, 0.f,
    -0.5f, 0.5f,-0.5f,  -1.f, 0.f, 0.f,
    -0.5f, 0.5f, 0.5f,  -1.f, 0.f, 0.f,
    -0.5f, 0.5f, 0.5f,  -1.f, 0.f, 0.f,
    -0.5f,-0.5f, 0.5f,  -1.f, 0.f, 0.f,
    -0.5f,-0.5f,-0.5f,  -1.f, 0.f, 0.f,

    // --- Right face (x = +0.5), normal (1,0,0)
     0.5f,-0.5f,-0.5f,   1.f, 0.f, 0.f,
     0.5f, 0.5f,-0.5f,   1.f, 0.f, 0.f,
     0.5f, 0.5f, 0.5f,   1.f, 0.f, 0.f,
     0.5f, 0.5f, 0.5f,   1.f, 0.f, 0.f,
     0.5f,-0.5f, 0.5f,   1.f, 0.f, 0.f,
     0.5f,-0.5f,-0.5f,   1.f, 0.f, 0.f,

     // --- Bottom face (y = -0.5), normal (0,-1,0)
     -0.5f,-0.5f,-0.5f,   0.f,-1.f, 0.f,
      0.5f,-0.5f,-0.5f,   0.f,-1.f, 0.f,
      0.5f,-0.5f, 0.5f,   0.f,-1.f, 0.f,
      0.5f,-0.5f, 0.5f,   0.f,-1.f, 0.f,
     -0.5f,-0.5f, 0.5f,   0.f,-1.f, 0.f,
     -0.5f,-0.5f,-0.5f,   0.f,-1.f, 0.f,

     // --- Top face (y = +0.5), normal (0,1,0)
     -0.5f, 0.5f,-0.5f,   0.f, 1.f, 0.f,
      0.5f, 0.5f,-0.5f,   0.f, 1.f, 0.f,
      0.5f, 0.5f, 0.5f,   0.f, 1.f, 0.f,
      0.5f, 0.5f, 0.5f,   0.f, 1.f, 0.f,
     -0.5f, 0.5f, 0.5f,   0.f, 1.f, 0.f,
     -0.5f, 0.5f,-0.5f,   0.f, 1.f, 0.f,
};

Cube::Cube() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
        (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Cube::~Cube() {
    if (VBO) glDeleteBuffers(1, &VBO);
    if (VAO) glDeleteVertexArrays(1, &VAO);
}

void Cube::Draw() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
