#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

glm::vec3 hexColor(unsigned int hex);

inline glm::vec3 hexRGB(unsigned int hex) {
    float r = ((hex >> 16) & 0xFF) / 255.0f;
    float g = ((hex >> 8) & 0xFF) / 255.0f;
    float b = (hex & 0xFF) / 255.0f;
    return glm::vec3(r, g, b);
}


struct FurnitureContext {
    unsigned int cubeVAO; // bound cube VBO/VAO with 36 verts, pos+normal
    Shader* shader;       // active shader (has 'model' and 'objectColor')
};

inline void setModel(Shader& sh, const glm::mat4& M) {
    sh.setMat4("model", M);
}

// ---------------- Coffee Table ----------------
inline void drawCoffeeTable(const FurnitureContext& ctx,
    const glm::vec3& worldPos = glm::vec3(0.0f),
    float rotateYdeg = 0.0f,
    const glm::vec3& globalScale = glm::vec3(1.0f))
{
    Shader& shader = *ctx.shader;
    glBindVertexArray(ctx.cubeVAO);

    // --- component sizes (in local space) ---
    glm::vec3 topScale = glm::vec3(2.0f, 0.15f, 1.2f);
    glm::vec3 legScale = glm::vec3(0.15f, 0.50f, 0.15f);

    // legs sit on floor (y=0), so their centers are at y = legScale.y * 0.5
    const float legHalf = legScale.y * 0.5f;
    const float topHalf = topScale.y * 0.5f;

    // local origin is at the leg center point
    glm::vec3 legCenter(0.0f, legHalf, 0.0f);
    glm::vec3 topPos = legCenter + glm::vec3(0.0f, legHalf + topHalf, 0.0f);

    // base transform for whole table
    glm::mat4 M = glm::mat4(1.0f);
    M = glm::translate(M, worldPos);
    M = glm::rotate(M, glm::radians(rotateYdeg), glm::vec3(0, 1, 0));
    M = glm::scale(M, globalScale);

    glm::vec3 topColor = hexRGB(0x808080);
    glm::vec3 legColor = hexRGB(0x808080);


    // tabletop
    {
        glm::mat4 model = M;
        model = glm::translate(model, topPos);
        model = glm::scale(model, topScale);
        shader.setMat4("model", model);
        shader.setVec3("objectColor", topColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // four legs (offsets from local center)
    float xOffset = topScale.x * 0.5f - legScale.x * 0.5f;
    float zOffset = topScale.z * 0.5f - legScale.z * 0.5f;
    glm::vec3 legOffsets[4] = {
        { xOffset, 0.0f,  zOffset},
        {-xOffset, 0.0f,  zOffset},
        { xOffset, 0.0f, -zOffset},
        {-xOffset, 0.0f, -zOffset}
    };

    for (int i = 0; i < 4; ++i) {
        glm::mat4 model = M;
        model = glm::translate(model, legCenter + legOffsets[i]);
        model = glm::scale(model, legScale);
        shader.setMat4("model", model);
        shader.setVec3("objectColor", legColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

// ---------------- TV Stand ----------------
inline void drawTVStand(const FurnitureContext& ctx,
    const glm::vec3& bodyPos = glm::vec3(0.0f, 0.50f, -2.2f)) {
        Shader& shader = *ctx.shader;
        glBindVertexArray(ctx.cubeVAO);

        glm::vec3 bodySize(1.6f, 0.50f, 0.50f);
        int shelves = 2;
        float shelfT = 0.04f;
        float legH = 0.10f, legT = 0.06f;

        glm::vec3 bodyColor(193.f / 255.f, 183.f / 255.f, 170.f / 255.f);
        glm::vec3 shelfColor(143.f / 255.f, 138.f / 255.f, 130.f / 255.f);
        glm::vec3 legColor(75.f / 255.f, 75.f / 255.f, 75.f / 255.f);

    // legs
    {
        float lx = bodySize.x * 0.5f - legT * 0.5f;
        float lz = bodySize.z * 0.5f - legT * 0.5f;
        float legY = bodyPos.y - (bodySize.y * 0.5f) - legH * 0.5f;

        glm::vec3 legCenters[4] = {
            { bodyPos.x + lx, legY, bodyPos.z + lz },
            { bodyPos.x - lx, legY, bodyPos.z + lz },
            { bodyPos.x + lx, legY, bodyPos.z - lz },
            { bodyPos.x - lx, legY, bodyPos.z - lz }
        };

        for (auto& c : legCenters) {
            glm::mat4 M(1.0f);
            M = glm::translate(M, c);
            M = glm::scale(M, glm::vec3(legT, legH, legT));
            setModel(shader, M);
            shader.setVec3("objectColor", legColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

    // body
    {
        glm::mat4 M(1.0f);
        M = glm::translate(M, bodyPos);
        M = glm::scale(M, bodySize);
        setModel(shader, M);
        shader.setVec3("objectColor", bodyColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // shelves
    for (int i = 1; i <= shelves; ++i) {
        float t = float(i) / float(shelves + 1);
        float y = bodyPos.y + (t - 0.5f) * bodySize.y;

        glm::mat4 M(1.0f);
        M = glm::translate(M, glm::vec3(bodyPos.x, y, bodyPos.z));
        M = glm::scale(M, glm::vec3(bodySize.x * 0.95f, shelfT, bodySize.z * 0.95f));
        setModel(shader, M);
        shader.setVec3("objectColor", shelfColor);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

// ---------------- Sofa ----------------
inline void drawSofa(const FurnitureContext& ctx,
    const glm::vec3& pos,    // world position of sofa center
    float yawDeg)            // rotate around Y so it faces table
{
    Shader& shader = *ctx.shader;
    glBindVertexArray(ctx.cubeVAO);

    // Colors
    glm::vec3 seatCol = hexColor(0x3E5F8A);
    glm::vec3 sideCol = hexColor(0x23374F);
    glm::vec3 legCol = hexColor(0x141922);

    // Base transform (translate then yaw rotate)
    auto T = glm::translate(glm::mat4(1.0f), pos);
    auto R = glm::rotate(glm::mat4(1.0f), glm::radians(yawDeg), glm::vec3(0, 1, 0));

    auto drawBox = [&](glm::vec3 localPos, glm::vec3 scale, glm::vec3 color)
        {
            glm::mat4 M = T * R;
            M = glm::translate(M, localPos);
            M = glm::scale(M, scale);
            shader.setMat4("model", M);
            shader.setVec3("objectColor", color);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        };

    // Dimensions (units match your room)
    // seat: 2.6m wide, 0.45m tall, 1.0m deep
    drawBox({ 0.0f, 0.225f, 0.0f }, { 2.6f, 0.45f, 1.0f }, seatCol);

    // backrest: sits at rear edge, 0.7m high, 0.12m thick
    drawBox({ 0.0f, 0.70f, -0.44f }, { 2.6f, 0.50f, 0.12f }, sideCol);

    // armrests: left/right
    drawBox({ -1.34f, 0.40f, 0.0f }, { 0.12f, 0.80f, 1.0f }, sideCol);
    drawBox({ 1.34f, 0.40f, 0.0f }, { 0.12f, 0.80f, 1.0f }, sideCol);

    // legs: four simple cubes
    const float hx = 1.20f, hz = 0.40f, ly = 0.10f;
    glm::vec3 legs[4] = {
        {-hx, ly * 0.5f,  hz}, {hx, ly * 0.5f,  hz},
        {-hx, ly * 0.5f, -hz}, {hx, ly * 0.5f, -hz}
    };
    for (auto& p : legs) drawBox(p, { 0.10f, ly, 0.10f }, legCol);
}
