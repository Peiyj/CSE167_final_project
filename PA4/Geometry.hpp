//
//  Geometry.hpp
//  PA0
//
//  Created by Karl Wang on 2/20/20.
//

#ifndef Geometry_hpp
#define Geometry_hpp

#include <stdio.h>
#include "Node.hpp"
#include "OBJObject.hpp"
class Geometry: public Node{
private:
    OBJObject* obj;
    GLuint modelLoc;
    GLuint program;
    GLint toneShading;
    
public:
    Geometry(const char* str, GLuint program);
    ~Geometry();
    void draw(const glm::mat4&);
    void update();
    void setLoc();
    void setColor(glm::vec3& color);
    void switchToToneShading();
};
#endif /* Geometry_hpp */
