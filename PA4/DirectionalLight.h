//
//  DirectionalLight.h
//  HW0
//
//  Created by Patrick on 2/12/20.
//  Copyright © 2020 Patrick. All rights reserved.
//

#ifndef _DIRECTIONALLIGHT_H_
#define _DIRECTIONALLIGHT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <vector>


class DirectionalLight{
private:
    float repScale;
    void getLoc();
    
public:
    float attenuation;
    GLuint positionLoc;
    GLuint attenuationLoc;
    GLuint repModelLoc;
    
    DirectionalLight(glm::vec3 color, glm::vec3 direction);
    ~DirectionalLight();
    
    void draw();
    void update();
    void spin(float deg);
    //color and direction
    glm::vec3 color;
    glm::vec3 direction;
};

#endif /* PointLight_h */



//#endif /* DirectionalLight_h */
