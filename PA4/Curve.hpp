//
//  Curve.hpp
//  PA0
//
//  Created by Karl Wang on 2/21/20.
//

#ifndef Curve_hpp
#define Curve_hpp
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <stdio.h>
class Curve{
private:
    GLuint vao;
    GLuint vbo;
    std::vector<glm::vec3> lines;
    GLuint modelLoc;
    GLuint colorLoc;
    glm::vec3 a;
    glm::vec3 b;
    glm::vec3 c;
    glm::vec3 d;
    glm::vec3 color;
public:
    Curve(GLuint, glm::vec3,glm::vec3,glm::vec3,glm::vec3);
    ~Curve();
    void draw();
    glm::vec3 calcBezier(float);
    
    
};
#endif /* Curve_hpp */
