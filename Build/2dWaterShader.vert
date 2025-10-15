#version 330 core
layout(location = 0) in vec3 aPos;  // Object vertex position
layout(location = 1) in vec3 aTexCoord;

uniform mat4 projection;
uniform vec2 playerPos;
uniform bool useWorldPos;

out vec4 clipSpace;
out vec2 textureCoords;
out vec3 textureCoords1;

out vec2 vSnappedUV;

void main() {
	float pixelWorldSize = 3.f;
	
	vec4 pos;
	if (useWorldPos)
		pos = vec4(aPos.x - playerPos.x, aPos.y - playerPos.y, 0.0, 1.0);
	else
		pos = vec4(aPos.x, aPos.y, 0.0, 1.0);

	vSnappedUV = floor(pos.xz / pixelWorldSize) * pixelWorldSize + pixelWorldSize * 0.5;

	clipSpace = projection * pos;
    gl_Position = clipSpace;//projection * view * worldPosition; // Normal MVP transformation
	textureCoords = vec2(aPos.x / 2.0f + 0.5f, aPos.z / 2.0f + 0.5f); // * 10
	textureCoords1 = aTexCoord;
}