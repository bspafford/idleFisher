#version 330 core
layout(location = 0) in vec3 aPos;  // Object vertex position
layout(location = 1) in vec3 inNormal;

uniform mat4 model;
uniform mat4 camMatrix;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

out vec4 clipSpace;
out vec2 textureCoords;

out vec2 vSnappedUV;

void main() {
	//vec3 crntPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
    //clipSpace = camMatrix * vec4(crntPos, 1.0);

	float pixelWorldSize = 3.f;
	
	vec4 worldPos = model * translation * rotation * scale * vec4(aPos, 1.0f);
	vSnappedUV = floor(worldPos.xz / pixelWorldSize) * pixelWorldSize + pixelWorldSize * 0.5;

	clipSpace = camMatrix * model * translation * rotation * scale * vec4(aPos, 1.0f);
    gl_Position = clipSpace;//projection * view * worldPosition; // Normal MVP transformation
	textureCoords = vec2(aPos.x / 2.0f + 0.5f, aPos.z / 2.0f + 0.5f); // * 10
}