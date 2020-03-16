#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
//in float sampleExtraOutput;
//in vec3 normal_vector;
in vec2 TexCoord;
in vec3 normal;
in float percentile;
in vec3 FragPos;
uniform sampler2D grass;
uniform sampler2D sand;
uniform sampler2D snow;


struct DirLight{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
uniform vec3 viewPos;
uniform int isToneShading;

const float levels = 4.0;
const vec3 grassColor = vec3(0.1, 0.5, 0);
const vec3 snowColor = vec3(1);
const vec3 sandColor = vec3(0.9, 0.9, 0.6);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 tex, float shininess,
                  vec3 specs)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = light.ambient  * tex;
    vec3 diffuse  = light.diffuse  * diff * tex;
    vec3 specular = light.specular * spec * specs;
    return (ambient + diffuse + specular);
}
vec3 CalcToneShading(DirLight light, vec3 normal, vec3 viewDir, vec3 color, vec3 specs,
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

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;
void main()
{
//    fragColor = 0;
//    fragColor = vec4(vec3(1.0f), 1.0f);
//    fragColor = mix(texture(grass, TexCoord), texture(sand, TexCoord), 0);
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - FragPos);


    vec3 tex;
    vec3 specular;
    vec3 color;
    float shininess;
    if(percentile > 0.9){
//        fragColor = texture(snow, TexCoord);
        color = snowColor;
        tex = vec3(texture(snow, TexCoord));
        specular = vec3(1);
        shininess = 4.0;
    }
    else if(percentile <= 0.9 && percentile > 0.7){
        color = (mix(grassColor, snowColor, (percentile-0.7)/0.2));
        tex = vec3(mix(texture(grass, TexCoord), texture(snow, TexCoord), (percentile - 0.7)/0.2));
        specular = vec3((percentile-0.7)/0.2);
        shininess = 4.0;
    }
    else if(percentile <= 0.7 && percentile > 0.3){
        color = grassColor;
        tex = vec3(texture(grass, TexCoord));
        specular = vec3(0);
        shininess = 0.0;
    }
    else if(percentile <= 0.3 && percentile > 0.1){
        color = (mix(sandColor, grassColor, (percentile-0.1)/0.2));
        tex = vec3(mix(texture(sand, TexCoord), texture(grass, TexCoord), (percentile - 0.1)/0.2));
        specular = vec3(0);
        shininess = 0.0;
    }
    else{
        color = sandColor;
        tex = vec3(texture(sand, TexCoord));
        specular = vec3(0);
        shininess = 0.0;
    }
    if(isToneShading == 1){
        fragColor = vec4(CalcToneShading(dirLight, normal, viewDir, color,  specular, norm), 1.0);
    }
    else{
        fragColor = vec4(CalcDirLight(dirLight, normal, viewDir, tex, shininess, specular), 1.0);
    }

}

