//
//  OBJObject.hpp
//  PA0
//
//  Created by Karl Wang on 2/7/20.
//

#ifndef OBJObject_hpp
#define OBJObject_hpp
#include "Object.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

class OBJObject : public Object{
private:
    GLuint vao;
    GLuint vbo[2];
    GLuint ebo;
    
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    
    
    GLuint program;
    GLuint modelLoc;
    GLuint colorLoc;
    
protected:
    glm::mat4 original_model;
    glm::mat4 moved_model;
    glm::mat4 scaled_model;
    glm::mat4 rotated_model;
public:

    OBJObject(const char*, GLuint);
    ~OBJObject();
    
    void draw();
    void update();
    void scaleModel(GLfloat scalar);
    void reset();
    

    void mouseRotate(float, glm::vec3);
    void moveAxis(GLfloat x, GLfloat y, GLfloat z);
    
    void setModel(glm::mat4 m) {this->model = m;}
    
    
};


#endif /* OBJObject_hpp */
