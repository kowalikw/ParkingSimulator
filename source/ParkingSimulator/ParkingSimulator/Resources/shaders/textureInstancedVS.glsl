#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 instancePosition;
layout (location = 4) in vec3 instanceRotation;
layout (location = 5) in vec3 instanceScale;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

mat4 transMtx = mat4(
	vec4(1, 0, 0, instancePosition.x),
	vec4(0, 1, 0, instancePosition.y),
	vec4(0, 0, 1, instancePosition.z),
	vec4(0, 0, 0, 1)
);

mat4 rotMtx = mat4(
	vec4(cos(instanceRotation.y), 0, sin(instanceRotation.y), 0),
	vec4(0, 1, 0, 0),
	vec4(-sin(instanceRotation.y), 0, cos(instanceRotation.y), 0),
	vec4(0, 0, 0, 1)
);

mat4 scaleMtx = mat4(
	vec4(instanceScale.x, 0, 0, 0),
	vec4(0, instanceScale.y, 0, 0),
	vec4(0, 0, instanceScale.z, 0),
	vec4(0, 0, 0, 1)
);

void main()
{
	if(abs(instanceRotation.y) < 10e-6)
	{
		gl_Position = projection * view * model * (vec4(position.xyz * instanceScale.xyz + instancePosition, 1.0f));
	}
	else
	{
		gl_Position = projection * view * model * (vec4(mat3(rotMtx) * (position.xyz * instanceScale.xyz) + instancePosition, 1.0f));
	}
	//gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoords = texCoords;
	FragPos = vec3(model * vec4(mat3(rotMtx) * (position.xyz * instanceScale.xyz) + instancePosition, 1.0f));
    Normal = mat3(transpose(inverse(transMtx * rotMtx * scaleMtx))) * normal;  
}