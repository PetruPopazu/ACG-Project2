#version 400

in vec2 textureCoord;
in vec3 norm;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec2 tiledCoords = textureCoord * 200;

	vec4 texColor = texture(texture_diffuse, tiledCoords);
	vec3 normalMapValue = texture(texture_normal, tiledCoords).rgb;

	vec3 ambient = 0.2f * lightColor;
	vec3 normal = normalize(norm);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = 0.5f * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * texColor.rgb;
	fragColor = vec4(result, texColor.a);
}
