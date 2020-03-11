#version 330 core
// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal_vector;
layout (location = 2) in vec2 aTexCoord;

// Uniform variables can be updated by fetching their location and passing values to that location

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float maxy;
uniform float miny;

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
//out float sampleExtraOutput;
out vec3 normal;
out vec2 TexCoord;
out float percentile;
out vec3 FragPos;

uniform vec4 plane;

float calcPercentile(float y){
    return (y-miny)/(maxy-miny);
}
void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    // tells the shader where the vertices should be rendered
    FragPos = vec3(model*vec4(position, 1.0));
    gl_ClipDistance[0] = dot(model*vec4(position, 1.0), plane);
//    gl_ClipDistance[0] = dot(model*vec4(position, 1.0), vec4(0, 1, 0, 10));
//    gl_ClipDistance[0] = -1;
    gl_Position = projection * view *model*vec4(position, 1.0) ;
    normal = mat3(transpose(inverse(model)))*normal_vector;
    TexCoord = aTexCoord*4;
    percentile = calcPercentile(position.y);
}

