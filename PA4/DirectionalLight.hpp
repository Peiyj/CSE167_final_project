//
//  DirectionalLight.hpp
//  PA0
//
//  Created by Karl Wang on 2/5/20.
//

#ifndef DirectionalLight_hpp
#define DirectionalLight_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <chrono>
#include "Light.hpp"



class DirectionalLight : public Light
{
private:
public:

    
    glm::vec3 direction;
    
    float degree = 0;
    
    
    
    GLuint ambientLoc;
    GLuint diffuseLoc;
    GLuint specularLoc;
    GLuint directionLoc;

    GLuint program;
    DirectionalLight(GLuint);
    ~DirectionalLight();
    
    void update();
    void draw();
    void bindToShader();
    void rotate(float degree);
};

#endif /* DirectionalLight_hpp */
