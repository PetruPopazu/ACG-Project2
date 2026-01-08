#version 400

in vec2 textureCoord; 
in vec3 norm;
in vec3 fragPos;

out vec4 fragColor;

// 1. We keep only two samplers now
uniform sampler2D texture_diffuse;  // Grass001_Diffuse.bmp
uniform sampler2D texture_normal;   // Grass001_Normal.bmp

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// Tiling: repeat the texture 50 times
	vec2 tiledCoords = textureCoord * 200.0;

	// 2. Sample only the two textures we kept
	vec4 texColor = texture(texture_diffuse, tiledCoords);
	// We load the normal map but for now we focus on basic Phong
	vec3 normalMapValue = texture(texture_normal, tiledCoords).rgb;

	// 3. Ambient Lighting
	float ambientStrength = 0.2;
	vec3 ambient = ambientStrength * lightColor;

	// 4. Diffuse Lighting
	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// 5. Specular Lighting (Using a fixed value instead of a map)
	float specularStrength = 0.5; // This replaces the roughness map
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	// Combine results
	vec3 result = (ambient + diffuse + specular) * texColor.rgb;
	fragColor = vec4(result, texColor.a);
}