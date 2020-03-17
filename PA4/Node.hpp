//
//  Node.hpp
//  PA0
//
//  Created by Karl Wang on 2/20/20.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>


class Node
{
private:
public:
    virtual void draw(const glm::mat4 &C)=0;
    virtual void update()=0;
    virtual void switchToToneShading()=0;
};
#endif /* Node_hpp */
