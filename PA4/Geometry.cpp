//
//  Geometry.cpp
//  PA0
//
//  Created by Karl Wang on 2/20/20.
//

#include "Geometry.hpp"
#include <iostream>
using namespace std;


Geometry::Geometry(const char* str, GLuint program){
    this->program = program;
    toneShading = 0;
    obj = new OBJObject(str, program);
    modelLoc = glGetUniformLocation(program, "model");
}
Geometry::~Geometry(){
    delete obj;
}
void Geometry::draw(const glm::mat4& C){
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(C*obj->getModel()));
    glUniform3fv(glGetUniformLocation(program, "color"), 1, glm::value_ptr(obj->getColor()));
    glUniform1i(glGetUniformLocation(program, "isToneShading"), toneShading);
    

    obj->draw();
}
void Geometry::update(){
    obj->update();
}
void Geometry::setColor(glm::vec3 &color){
    obj->setColor(color);
}
void Geometry::switchToToneShading(){
    toneShading = !toneShading;
}

