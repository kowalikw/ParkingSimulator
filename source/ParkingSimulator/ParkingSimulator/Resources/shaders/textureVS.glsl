#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 instancePosition;
layout (location = 4) in vec3 instanceRotation;
layout (location = 5) in vec3 instanceScale;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

mat3 rotMtx = mat3(
	vec3(cos(instanceRotation.y), 0, sin(instanceRotation.y)),
	vec3(0, 1, 0),
	vec3(-sin(instanceRotation.y), 0, cos(instanceRotation.y))
);

void main()
{
	if(abs(instanceRotation.y) < 10e-6)
	{
		gl_Position = projection * view * model * (vec4(position.xyz * instanceScale.xyz + instancePosition, 1.0f));
	}
	else
	{
		gl_Position = projection * view * model * (vec4(rotMtx * position.xyz * instanceScale.xyz + instancePosition, 1.0f));
	}
    TexCoords = texCoords;
}