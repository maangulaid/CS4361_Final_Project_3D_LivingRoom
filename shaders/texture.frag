#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D material_diffuse;
uniform bool useTexture;

void main()
{
    vec3 color;

    if (useTexture)
        color = texture(material_diffuse, TexCoord).rgb;
    else
        color = vec3(1.0, 1.0, 1.0);  // fallback

    FragColor = vec4(color, 1.0);
}
