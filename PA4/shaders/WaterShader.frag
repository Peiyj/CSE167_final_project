#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
//in vec3 normal_vector;
in vec2 TexCoord;
// You can output many things. The first vec4 type output determines the color of the fragment
uniform vec3 color;
uniform sampler2D tex;
out vec4 fragColor;
void main()
{

//    fragColor = vec4(0, 0.4, 0.8, 1.0);
    fragColor = texture(tex, TexCoord);

}

