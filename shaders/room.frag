#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform bool useTexture;

uniform vec3 objectColor;

void main() {
    vec3 result;

    if (useTexture)
        result = texture(diffuseTexture, TexCoord).rgb;
    else
        result = objectColor;

    FragColor = vec4(result,1.0);
}
