#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
//in float sampleExtraOutput;
in vec3 normal;

uniform vec3 color;
in vec3 FragPos;
struct DirLight{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
uniform vec3 viewPos;
uniform int isToneShading;
const float levels = 3.0;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;
vec3 CalcDirLight(DirLight light, vec3 viewDir, vec3 color, float shininess,
                  vec3 specs)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = light.ambient  * color;
    vec3 diffuse  = light.diffuse  * diff * color;
    vec3 specular = light.specular * spec * specs;
    return (ambient + diffuse + specular);
}
vec3 CalcToneShading(DirLight light, vec3 viewDir, vec3 color, vec3 specs,
                     vec3 norm)
{
    /* directional light */
    vec3 d_lightDir = normalize(-light.direction);
    float d_diff = max(dot(norm, d_lightDir), 0.0);
    vec3 d_reflectDir = reflect(-d_lightDir, norm);
    float d_spec = pow(max(dot(viewDir, d_reflectDir), 0.0), 0.1);
    
    vec3 d_ambient = light.ambient * color;
    vec3 d_diffuse = light.diffuse * d_diff * color;
    vec3 d_specular = light.specular * d_spec * specs;
    
    //modify specular and diffuse for toon shading effect
    float brightness = dot(viewDir, norm);
    brightness *= levels;
    brightness = floor(brightness+1.0);
    
    brightness /= levels;
    vec3 result = brightness * (d_ambient + d_diffuse + d_specular);
    return result;
}
void main()
{
//    fragColor = vec4(vec3(1.0f), 1.0f);
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 specular = vec3(1);
    float shininess = 8.0;
    vec3 planeColor;
    if(isToneShading == 1){
        planeColor = CalcToneShading(dirLight, viewDir, color, specular, norm);
    }
    else{
        planeColor = CalcDirLight(dirLight, viewDir, color, shininess,
                                   specular);
    }
//    fragColor = vec4(0.5f*normalize(normal)+0.5f, 1.0f);
    fragColor = vec4(planeColor, 1.0);

}

