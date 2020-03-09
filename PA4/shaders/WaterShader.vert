#version 330 core
// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;

// Uniform variables can be updated by fetching their location and passing values to that location

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 cameraPos;


out vec4 clipSpace;
out vec2 texCoord;
out vec3 toCameraVector;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
const float tiling=6.0;
void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    // tells the shader where the vertices should be rendered
    vec4 worldPosition = model * vec4(position, 1.0);
    clipSpace = projection * view * worldPosition;
    gl_Position = clipSpace;
    texCoord = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5)*tiling;
    toCameraVector = cameraPos - worldPosition.xyz;



}

