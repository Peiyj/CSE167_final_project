#version 330 core
// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;


// Uniform variables can be updated by fetching their location and passing values to that location

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 plane;
// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out vec3 TexCoords;

void main()
{
    gl_ClipDistance[0] = dot(model*vec4(position, 1.0), plane);
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    // tells the shader where the vertices should be rendered
    gl_Position = projection * view * model * vec4(position, 1.0);
    TexCoords = position;
}
