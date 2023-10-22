#version 430

in layout(location = 0) vec3 fposition;
in layout(location = 1) vec3 fnormal;
in layout(location = 2) vec2 ftexcoord;

out layout(location = 0) vec4 ocolor;

layout(binding = 0) uniform sampler2D tex;

uniform struct Material {
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec2 offset;
	vec2 tiling;
} material;

uniform struct Light {
	vec3 ambientLight;
	vec3 diffuseLight;
	vec3 lightPosition;
} light;

vec3 ads(in vec3 position, in vec3 normal)
{
	// AMBIENT
	vec3 ambient = light.ambientLight;
	// DIFFUSE
	vec3 lightDir = normalize(light.lightPosition - position);
	float intensity = max(dot(lightDir, normal), 0);

	vec3 diffuse = material.diffuse * (light.diffuseLight * intensity);

	// SPECULAR
	vec3 specular = vec3(0);
	if (intensity > 0)
	{
		vec3 reflection = reflect(-lightDir, normal);
		vec3 viewDir = normalize(-position);
		vec3 halfwayDir = normalize(lightDir + viewDir);
		intensity = max(dot(normal, halfwayDir), 0);
		intensity = pow(intensity, material.shininess);
		specular = material.specular * intensity;
	}

	return ambient + diffuse + specular;
}

void main()
{
	vec4 texcolor = texture(tex, ftexcoord);
	ocolor = texcolor * vec4(ads(fposition, fnormal), 1);
}