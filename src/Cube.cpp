#include "Cube.h"

// position (3) | normal (3) | uv (2)  = 8 floats per vertex
static float cubeVertices[] = {
    // back face
    -0.5f, -0.5f, -0.5f,  0,0,-1,   0,0,
     0.5f, -0.5f, -0.5f,  0,0,-1,   1,0,
     0.5f,  0.5f, -0.5f,  0,0,-1,   1,1,
     0.5f,  0.5f, -0.5f,  0,0,-1,   1,1,
    -0.5f,  0.5f, -0.5f,  0,0,-1,   0,1,
    -0.5f, -0.5f, -0.5f,  0,0,-1,   0,0,

    // front face
    -0.5f,-0.5f,0.5f,   0,0,1,   0,0,
     0.5f,-0.5f,0.5f,   0,0,1,   1,0,
     0.5f,0.5f,0.5f,    0,0,1,   1,1,
     0.5f,0.5f,0.5f,    0,0,1,   1,1,
    -0.5f,0.5f,0.5f,    0,0,1,   0,1,
    -0.5f,-0.5f,0.5f,   0,0,1,   0,0,

    // left face
    -0.5f,0.5f,0.5f,  -1,0,0,   1,1,
    -0.5f,0.5f,-0.5f, -1,0,0,   0,1,
    -0.5f,-0.5f,-0.5f,-1,0,0,   0,0,
    -0.5f,-0.5f,-0.5f,-1,0,0,   0,0,
    -0.5f,-0.5f,0.5f,  -1,0,0,   1,0,
    -0.5f,0.5f,0.5f,  -1,0,0,   1,1,

    // right face
 0.5f,0.5f,0.5f,   1,0,0,   0,1,
 0.5f,0.5f,-0.5f,  1,0,0,   1,1,
 0.5f,-0.5f,-0.5f, 1,0,0,   1,0,
 0.5f,-0.5f,-0.5f, 1,0,0,   1,0,
 0.5f,-0.5f,0.5f,  1,0,0,   0,0,
 0.5f,0.5f,0.5f,   1,0,0,   0,1,


    // bottom
    -0.5f,-0.5f,-0.5f, 0,-1,0,  0,1,
     0.5f,-0.5f,-0.5f, 0,-1,0,  1,1,
     0.5f,-0.5f,0.5f,  0,-1,0,  1,0,
     0.5f,-0.5f,0.5f,  0,-1,0,  1,0,
    -0.5f,-0.5f,0.5f,  0,-1,0,  0,0,
    -0.5f,-0.5f,-0.5f, 0,-1,0,  0,1,

    // top
    -0.5f,0.5f,-0.5f,  0,1,0,   0,1,
     0.5f,0.5f,-0.5f,  0,1,0,   1,1,
     0.5f,0.5f,0.5f,   0,1,0,   1,0,
     0.5f,0.5f,0.5f,   0,1,0,   1,0,
    -0.5f,0.5f,0.5f,   0,1,0,   0,0,
    -0.5f,0.5f,-0.5f,  0,1,0,   0,1
};

Cube::Cube() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    // normals
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // uv
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
}

Cube::~Cube() {
    glDeleteBuffers(1,&VBO);
    glDeleteVertexArrays(1,&VAO);
}

void Cube::Draw() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES,0,36);
}
