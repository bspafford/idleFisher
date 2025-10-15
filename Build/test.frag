#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D shadowMap;

void main() {
    float depth = texture(shadowMap, TexCoords).r; // Sample depth value
    FragColor = vec4(vec3(depth), 1.0); // Convert to grayscale
}