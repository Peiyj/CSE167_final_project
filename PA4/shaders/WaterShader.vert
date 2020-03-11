#version 330 core
// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 normal_neighbor;
layout (location = 2) in vec2 TexCoord;

// Uniform variables can be updated by fetching their location and passing values to that location

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 cameraPos;
uniform float waveTime;


out vec4 clipSpace;
out vec2 texCoord;
out vec3 toCameraVector;
out vec4 clipSpaceGrid;
out vec3 normal;


out vec4 worldPosition;



// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
const float tiling=5;


const float PI = 3.1415926535897932384626433832795;
const float waveLength = 10.0;
const float waveAmplitude = 2.0;


float generateOffset(float x, float z){
    float radiansX = (x/waveLength + waveTime) *2.0*PI;
    float radiansZ = (z/waveLength + waveTime) *2.0*PI;
    return waveAmplitude *0.5 *(sin(radiansZ) + cos(radiansX));
}

vec3 applyDistortion(vec3 vertex){
    float xDistortion = generateOffset(vertex.x, vertex.z);
    float yDistortion = generateOffset(vertex.x, vertex.z);
    float zDistortion = generateOffset(vertex.x, vertex.z);
    return vertex + vec3(xDistortion + yDistortion + zDistortion);
}

vec3 calcNormal(vec3 vertex0, vec3 vertex1, vec3 vertex2){
    vec3 tangent = vertex1 - vertex0;
    vec3 bitangent = vertex2 - vertex0;
    return normalize(cross(tangent, bitangent));
    
}

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    // tells the shader where the vertices should be rendered
    
    
    vec3 vertex1 = position + vec3(normal_neighbor.x, 0, normal_neighbor.y);
    vec3 vertex2 = position + vec3(normal_neighbor.z, 0, normal_neighbor.w);
    clipSpaceGrid = projection*view*model*vec4(position, 1.0);
    vec3 distortedPos = applyDistortion(position);
    
//    vec3 distortedPos = position;
    vertex1 = applyDistortion(vertex1);
    vertex2 = applyDistortion(vertex2);
    
    
    normal = calcNormal(distortedPos, vertex1, vertex2);
    normal = mat3(transpose(inverse(model)))*normal;

//    vec4 worldPosition = model * vec4(position, 1.0);
    worldPosition = model * vec4(distortedPos, 1.0);
    clipSpace = projection * view * worldPosition;
    gl_Position = clipSpace;
    // texCoord is assuming position are from 0 to 1
//    texCoord = vec2(position.x/2.0 + 0.5, position.z/2.0 + 0.5)*tiling;
    texCoord = TexCoord;
    toCameraVector = cameraPos - worldPosition.xyz;



}

