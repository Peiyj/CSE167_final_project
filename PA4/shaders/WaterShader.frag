#version 330 core
// This is a sample fragment shader.


in vec4 clipSpace;
in vec2 texCoord;
in vec3 toCameraVector;


uniform sampler2D reflectionTex;
uniform sampler2D refractionTex;
uniform sampler2D dudvMap;

uniform float moveFactor;


out vec4 fragColor;
const float waveStrength = 0.02;

void main()
{
    
    vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
    vec2 refractTexCoord = vec2(ndc.x, ndc.y);
    vec2 reflectTexCoord = vec2(ndc.x, -ndc.y);
    
    
    vec2 distortion1 = (texture(dudvMap, vec2(texCoord.x+moveFactor, texCoord.y)).rg * 2.0 - 1.0);
    distortion1 *= waveStrength;
    vec2 distortion2 = (texture(dudvMap, vec2(-texCoord.x+moveFactor, texCoord.y+moveFactor)).rg * 2.0 - 1.0);
    distortion2 *= waveStrength;
    vec2 sum = distortion1 + distortion2;
    
    refractTexCoord += sum;
    refractTexCoord = clamp(refractTexCoord, 0.001, 0.999);
    
    
    
    
    reflectTexCoord += sum;
    reflectTexCoord.x = clamp(reflectTexCoord.x, 0.001, 0.999);
    reflectTexCoord.y = clamp(reflectTexCoord.y, -0.999, -0.001);

    vec4 reflectionColor = texture(reflectionTex, reflectTexCoord);
    vec4 refractionColor = texture(refractionTex, refractTexCoord);
//    fragColor = vec4(0, 0.4, 0.8, 1.0);
    
    
    
    vec3 viewVector = normalize(toCameraVector);
    float refractiveFactor = dot(viewVector, vec3(0,1,0));
    refractiveFactor = pow(refractiveFactor, 4);
    fragColor = mix(reflectionColor, refractionColor, refractiveFactor);
    fragColor = mix(fragColor, vec4(0, 0.3, 0.5, 1.0), 0.4);

}

