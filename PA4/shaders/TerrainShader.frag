#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
//in float sampleExtraOutput;
//in vec3 normal_vector;
in vec2 TexCoord;
uniform vec3 color;
uniform sampler2D ourTexture;
// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;
void main()
{
//    fragColor = vec4(vec3(1.0f), 1.0f);
    fragColor = texture(ourTexture, TexCoord);

}

