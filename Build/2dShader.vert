#version 330 core
layout (location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 projection;
uniform vec2 playerPos;
uniform bool useWorldPos;


void main() {
    if (useWorldPos) {
        vec4 worldPosition = vec4(aPos.x - playerPos.x, aPos.y - playerPos.y, 0.0, 1.0);
        gl_Position = projection * worldPosition;
    } else {
        gl_Position = projection * vec4(aPos, 0, 1);
    }

    TexCoord = aTexCoord;
}