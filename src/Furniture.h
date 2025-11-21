#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

// hexColor is defined in main.cpp; we just declare it here
glm::vec3 hexColor(unsigned int hex);

struct FurnitureContext {
    unsigned int cubeVAO;
    Shader* shader;
};

// Draw a simple coffee table made from cubes
inline void drawCoffeeTable(const FurnitureContext& ctx) {
    Shader& shader = *ctx.shader;
    glBindVertexArray(ctx.cubeVAO);

    // ---------- table parameters ----------
    // Sizes
    glm::vec3 topScale = glm::vec3(2.0f, 0.15f, 1.2f);    // tabletop
    glm::vec3 legScale = glm::vec3(0.15f, 0.5f, 0.15f);   // legs

    // Center of the legs in world space.
    // Legs are 0.5 units tall, so center at y = 0.25 puts the bottoms on the floor (y = 0).
    glm::vec3 legCenter = glm::vec3(0.0f, legScale.y * 0.5f, -1.5f);

    // Colors
    glm::vec3 topColor = hexColor(0xB07C4D);  // warm wood
    glm::vec3 legColor = hexColor(0x8B5A2B);  // darker legs

    // ---------- tabletop ----------
    // Tabletop should sit exactly on top of the legs:
    //
    // leg top y  = legCenter.y + legScale.y / 2
    // top center y = leg top y + topScale.y / 2
    //
    float legHalf = legScale.y * 0.5f;
    float topHalf = topScale.y * 0.5f;

    glm::vec3 topPos = legCenter + glm::vec3(0.0f, legHalf + topHalf, 0.0f);

    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, topPos);
        model = glm::scale(model, topScale);

        shader.setMat4("model", model);
        shader.setVec3("objectColor", topColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // ---------- legs ----------
    // Offsets in X/Z from the leg center to the four corners
    float xOffset = topScale.x * 0.5f - legScale.x * 0.5f;
    float zOffset = topScale.z * 0.5f - legScale.z * 0.5f;

    glm::vec3 legOffsets[4] = {
        glm::vec3( xOffset, 0.0f,  zOffset),
        glm::vec3(-xOffset, 0.0f,  zOffset),
        glm::vec3( xOffset, 0.0f, -zOffset),
        glm::vec3(-xOffset, 0.0f, -zOffset)
    };

    for (int i = 0; i < 4; ++i) {
        glm::mat4 model = glm::mat4(1.0f);
        // each leg is at legCenter + its offset
        glm::vec3 pos = legCenter + legOffsets[i];
        model = glm::translate(model, pos);
        model = glm::scale(model, legScale);

        shader.setMat4("model", model);
        shader.setVec3("objectColor", legColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
