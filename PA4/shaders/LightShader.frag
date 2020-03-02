#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 FragPos;
in vec3 Normal;

uniform vec3 color;

uniform vec3 cameraPos;
uniform vec3 dlightColor;
uniform vec3 dlightDirection;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    // shared source code
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - FragPos);
    
    /* directional light */
    vec3 d_lightDir = normalize(-dlightDirection);
    float d_diff = max(dot(norm, d_lightDir), 0.0);
    vec3 d_reflectDir = reflect(-d_lightDir, norm);
    float d_spec = pow(max(dot(viewDir, d_reflectDir), 0.0), 0.1);
    
    vec3 d_ambient = dlightColor * vec3(0.135,0.2225,0.1575);
    vec3 d_diffuse = dlightColor * d_diff * vec3(0.54,0.89,0.63);
    vec3 d_specular = dlightColor * d_spec * vec3(0.316228,0.316228, 0.316228);
    
    //modify specular and diffuse for toon shading effect
    float edgeDetection = (dot(viewDir, norm) > 0.2) ? 1 : 0;
    vec3 result = edgeDetection * (d_ambient + d_diffuse + d_specular);
    fragColor = vec4 (result, 1.0);
}
