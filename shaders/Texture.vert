#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // World-space position of this vertex
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Normal transformed into world space
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Pass UV coordinates to fragment shader
    TexCoord = aTexCoord;

    // Final clip-space position
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
