#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform int isRectangle;
uniform vec4 color;

uniform sampler2D texture1;

void main() {
    if (isRectangle == 1) {
        FragColor = color;
        return;
    }
    
    vec4 textureColor = texture(texture1, TexCoord);
    FragColor = textureColor * color;
}