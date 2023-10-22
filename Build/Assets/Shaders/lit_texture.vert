#version 430

in layout(location = 0) vec3 vposition;
in layout(location = 1) vec3 vnormal;
in layout(location = 2) vec2 vtexcoord;

out layout(location = 0) vec3 oposition;
out layout(location = 1) vec3 onormal;
out layout(location = 2) vec2 otexcoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform struct Material {
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec2 offset;
	vec2 tiling;
} material;

void main()
{
	oposition = vec3(model * vec4(vposition, 1));
	onormal = mat3(transpose(inverse(model))) * vnormal;
	otexcoord = vtexcoord;


	mat4 mvp = projection * view * model;
	gl_Position = mvp * vec4(vposition, 1.0);
}
