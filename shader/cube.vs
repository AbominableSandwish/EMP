#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 0) in vec3 aNormal;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
   FragPos = vec3(transform * vec4(aPos, 1.0));
   Normal = mat3(transpose(inverse(transform))) * aNormal;

   gl_Position = projection * view * transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}