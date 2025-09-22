#version 330 core

in vec2 textureCoords;
in vec4 clipSpace;

out vec4 FragColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvMap;

uniform float moveFactor;

const float waveStrength = 0.02f;

in vec2 vSnappedUV;

void main() {
    vec2 screenResolution = vec2(1920, 1080);
	float pixelSize = 3.0;
	
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.f + 0.5f;
	vec2 pixelatedNDC = floor(ndc * screenResolution / pixelSize) * pixelSize / screenResolution;

	vec2 refractTexCoords = pixelatedNDC;
	vec2 reflectTexCoords = vec2(pixelatedNDC.x, pixelatedNDC.y);

	vec2 dudvUV = textureCoords;
	vec2 distortion1 = (texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y )).rg * 2.0f - 1.0f) * waveStrength;
	refractTexCoords += distortion1;
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

	reflectTexCoords += distortion1;
	reflectTexCoords = clamp(reflectTexCoords, 0.001, 0.999);

	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, reflectTexCoords);

	//FragColor = mix(reflectColor, refractColor, 0.5);
	FragColor = reflectColor;

	FragColor = vec4(0);

/*
	vec2 screenResolution = vec2(1920, 1080);
	float pixelSize = 3.0;

    // Snap fragment position to 3x3 screen pixel blocks
    vec2 snappedCoord = floor(gl_FragCoord.xy / pixelSize) * pixelSize + vec2(0.5); // center of pixel block
    vec2 pixelatedNDC = snappedCoord / screenResolution;

    // Base UVs (same for refract and reflect)
    vec2 baseUV = pixelatedNDC;

    // Use snapped UVs to sample distortion too — prevents shimmering
    vec2 dudv = texture(dudvMap, vec2(baseUV.x + moveFactor, baseUV.y)).rg;
    vec2 distortion = (dudv * 2.0 - 1.0) * waveStrength;

    vec2 refractTexCoords = clamp(baseUV + distortion, 0.001, 0.999);
    vec2 reflectTexCoords = clamp(baseUV + distortion, 0.001, 0.999);

    vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
    vec4 refractColor = texture(refractionTexture, refractTexCoords);

    FragColor = mix(reflectColor, refractColor, 0.5);

	*/
}

/*


	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.f + 0.5f;
	vec2 refractTexCoords = ndc.xy;
	vec2 reflectTexCoords = vec2(ndc.x, ndc.y);

	vec2 distortion1 = (texture(dudvMap, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 2.0f - 1.0f) * waveStrength;
	refractTexCoords += distortion1;
	// clamp so doesn't wrap and glitch / look weird
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);

	reflectTexCoords += distortion1;
	reflectTexCoords = clamp(reflectTexCoords, 0.001, 0.999);


	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);

	FragColor = mix(reflectColor, refractColor, 0.5);
	//FragColor = texture(dudvMap, textureCoords);


*/