#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D material_diffuse;

void main() {
    FragColor = texture(material_diffuse, vUV);
}
