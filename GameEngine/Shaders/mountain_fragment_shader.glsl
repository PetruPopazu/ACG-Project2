#version 400
out vec4 FragColor;

in vec2 textureCoord;
in vec3 norm;
in vec3 fragPos;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_normal;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec2 tiledCoord = textureCoord * 20.0;

	vec4 textureColor = texture(texture_diffuse, tiledCoord);
	vec3 normal = normalize(norm);

	vec3 ambient = 0.3 * lightColor;
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = 0.2 * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * textureColor.rgb;
	FragColor = vec4(result, 1.0);
}