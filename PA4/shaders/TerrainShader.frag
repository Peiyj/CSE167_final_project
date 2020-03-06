#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
//in float sampleExtraOutput;
//in vec3 normal_vector;
in vec2 TexCoord;
in float percentile;
uniform sampler2D grass;
uniform sampler2D sand;
uniform sampler2D snow;
// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;
void main()
{
//    fragColor = 0;
//    fragColor = vec4(vec3(1.0f), 1.0f);
//    fragColor = mix(texture(grass, TexCoord), texture(sand, TexCoord), 0);
    if(percentile > 0.9){
        fragColor = texture(snow, TexCoord);
    }
    else if(percentile <= 0.9 && percentile > 0.7){
        fragColor = mix(texture(grass, TexCoord), texture(snow, TexCoord), (percentile - 0.7)/0.2);
    }
    else if(percentile <= 0.7 && percentile > 0.3){
        fragColor = texture(grass, TexCoord);
    }
    else if(percentile <= 0.3 && percentile > 0.1){
        fragColor = mix(texture(sand, TexCoord), texture(grass, TexCoord), (percentile - 0.1)/0.2);
    }
    else{
        fragColor = texture(sand, TexCoord);
    }

}

