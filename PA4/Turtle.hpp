//
//  Turtle.hpp
//  PA0
//
//  Created by Karl Wang on 3/15/20.
//

#ifndef Turtle_hpp
#define Turtle_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <float.h>
#include <stdlib.h>     /* srand, rand */
class Turtle{
private:
    float currAngle;
    glm::vec3 currPos;
    GLuint vao;
    GLuint vbo;
    std::vector<glm::vec3> lines;
    
    
public:
    void drawLine();
    Turtle();
    ~Turtle();
    
};
#endif /* Turtle_hpp */
