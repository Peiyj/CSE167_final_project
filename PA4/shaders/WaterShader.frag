#version 330 core
// This is a sample fragment shader.


in vec4 clipSpace;
in vec2 texCoord;
in vec3 toCameraVector;
in vec4 clipSpaceGrid;
in vec3 normal;
in vec4 worldPosition;


const vec3 waterColor = vec3(0.604, 0.867, 0.851);
//const vec3 waterColor = vec3(0, 0.3, 0.5);
const float minBlueness = 0;
const float maxBlueness = 0.7;
const float murkyDepth = 100;


uniform sampler2D reflectionTex;
uniform sampler2D refractionTex;
uniform sampler2D dudvMap;

uniform sampler2D depthMap;



uniform int isNormalColor;
uniform int showDepthMap;
uniform int noDistortion;
uniform float moveFactor;

uniform vec3 viewPos;
out vec4 fragColor;
const float waveStrength = 0.03;
const vec3 spec_component = vec3(1.0, 1.0, 1.0);
const float shininess = 800;




struct DirLight{
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float shininess,
                  vec3 specs)
{
    vec3 lightDir = normalize(light.direction);
    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
//    vec3 ambient  = light.ambient  * tex;
//    vec3 diffuse  = light.diffuse  * diff * tex;
    vec3 specular = light.specular * spec * specs;
//    return (ambient + diffuse + specular);
    return specular;
}



vec3 applyMurkiness(vec3 refractColor, float waterDepth){
    float murkyFactor = smoothstep(0, murkyDepth, waterDepth);
    float murkiness = minBlueness + murkyFactor * (maxBlueness - minBlueness);
    return mix(refractColor, waterColor, murkiness);
}



void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - vec3(worldPosition));

    vec3 specular = CalcDirLight(dirLight, normal, viewDir, shininess, spec_component);
    
    
    
    
    
    vec2 ndcGrid = (clipSpaceGrid.xy/clipSpaceGrid.w)/2.0 + 0.5;
    ndcGrid = clamp(ndcGrid, 0.002, 0.998);
    vec2 refractTexCoord = vec2(ndcGrid.x, ndcGrid.y);
    vec2 reflectTexCoord = vec2(ndcGrid.x, -ndcGrid.y);

    
    
    
    
    
    
    
//    vec5 temp = clipSpace;
    vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
//    ndc = ndcGrid;

    float near = 1;
    float far = 5000;
//    float depth = texture(depthMap, refractTexCoord).r;
    float depth = texture(depthMap, ndc).r;
    float floorDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
    
    
    // check depth, make edges softer using transparency aka alpha blending
    depth = gl_FragCoord.z;
    float waterDistance = 2.0 * near * far / (far + near - (2.0 * depth - 1.0) * (far - near));
    
    
    float waterDepth = floorDistance - waterDistance;

    
    
    
    
    
    // make distortion for repling effect, linearize depth
    vec2 distortion1 = (texture(dudvMap, vec2(texCoord.x+moveFactor, texCoord.y)).rg * 2.0 - 1.0);
    distortion1 *= waveStrength;
    vec2 distortion2 = (texture(dudvMap, vec2(-texCoord.x+moveFactor, texCoord.y+moveFactor)).rg * 2.0 - 1.0);
    distortion2 *= waveStrength;
    vec2 sum = distortion1 + distortion2;
    sum *= clamp(waterDepth/20.0, 0.0, 1.0);
    

    if(noDistortion == 1){
        sum = vec2(0);
    }
    
    
    refractTexCoord += sum;
    refractTexCoord = clamp(refractTexCoord, 0.001, 0.999);
    
    
    
    
    reflectTexCoord += sum;
    reflectTexCoord.x = clamp(reflectTexCoord.x, 0.001, 0.999);
    reflectTexCoord.y = clamp(reflectTexCoord.y, -0.999, -0.001);

    vec3 reflectionColor = texture(reflectionTex, reflectTexCoord).rgb;
    vec3 refractionColor = texture(refractionTex, refractTexCoord).rgb;
    
    
    refractionColor = applyMurkiness(refractionColor, waterDepth);

    
    
    vec3 viewVector = normalize(toCameraVector);
//    float refractiveFactor = dot(viewVector, vec3(0,1,0));
    float refractiveFactor = dot(viewVector, normal);
    // higher power means more reflection
    refractiveFactor = pow(refractiveFactor, 2);
    vec3 color = mix(reflectionColor, refractionColor, refractiveFactor);
    
    
    
//    color = color+specular;
    
    
    
    fragColor = vec4(color, 1.0);

    
    // make water blueish
//    fragColor = mix(fragColor, vec4(0, 0.3, 0.5, 1.0), 0.4);
   
    
    // make soft edges
    fragColor.a = clamp(waterDepth/20.0, 0.0, 1.0);
    fragColor = isNormalColor*vec4(normal*5, 1.0) + (1-isNormalColor) * fragColor;
    
    
    //test if depth works
    if(showDepthMap == 1){
        fragColor = vec4(vec3(waterDepth/200), 1.0);
    }

}

