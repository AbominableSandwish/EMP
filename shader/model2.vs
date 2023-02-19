#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

uniform mat4 transform;
layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec3 FragPos;
out vec3 Normal;

void main()
{
   FragPos = vec3(transform * vec4(aPos, 1.0));
   Normal = mat3(transpose(inverse(transform))) * aNormal;

   gl_Position = projection * view * vec4(FragPos, 1.0);
}