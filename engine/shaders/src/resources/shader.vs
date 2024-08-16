#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
uniform float offset;
out vec4 vertex_color;

void main()
{
    //gl_Position = vertex_color = vec4((aPos.x * (-1)) - 1 + offset, (aPos.y * (-1)) + 1, aPos.z, 1.0f);
    gl_Position = vertex_color = vec4(aPos.x , aPos.y - offset, aPos.z, 1.0f);
    //vertex_color = vec4(aColor, 1.0);
} 
