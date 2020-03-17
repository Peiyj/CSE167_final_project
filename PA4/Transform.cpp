//
//  Transform.cpp
//  PA0
//
//  Created by Karl Wang on 2/20/20.
//

#include "Transform.hpp"
#include <iostream>
using namespace std;
Transform::Transform(const glm::mat4& M){
    this->M = M;
    original_M = M;
    scaled_M = glm::mat4(1);
    moved_M = glm::mat4(1);
    rotated_M = glm::mat4(1);
    walking = false;
    moving = false;
}
Transform::~Transform(){
    for(Node* child: children){
        delete child;
    }
    
}
void Transform::addChild(Node* n){
    children.push_back(n);
}
void Transform::removeChild(Node* n){
    for(int i = 0; i < children.size(); i++){
        if(children[i] == n){
            children.erase(children.begin()+i);
            return;
        }
    }
    cout<< "child not found" << endl;
    return;
}
void Transform::draw(const glm::mat4 &C){
    glm::mat4 new_T = C*M;
    for(Node* child: children){
        child->draw(new_T);
    }
}
void Transform::update(){
//    M = rotated_M*moved_M*glm::mat4(1);
    M = moved_M*rotated_M* scaled_M* glm::mat4(1);
//    M = rotated_M*glm::mat4(1);
}
void Transform::startWalking(int forward){
    this->forward = forward;
    rotateMatrix(-20);
    degree = 0;
}
void Transform::rotateMatrix(float degree){
    this->degree += degree;
    M = glm::rotate(glm::mat4(1), glm::radians(degree), glm::vec3(1,0,0))*M;
//    M = glm::rotate(M, glm::radians(degree), glm::vec3(1,0,0));
}
void Transform::scaleMatrix(float amount){
    scaled_M = glm::scale(glm::mat4(1), glm::vec3(amount))*scaled_M;
}
void Transform::mouseRotate(float rot_angle, glm::vec3 (rotAxis)){
    rotated_M = glm::rotate(glm::mat4(1.0f), glm::radians(rot_angle), rotAxis) * rotated_M;
}
void Transform::moveMatrix(glm::vec3& pos){
//    moving = true;
    moved_M = glm::translate(glm::mat4(1), pos);
//    update();
}
void Transform::updateRotation(){
    rotateMatrix(1);
}


void Transform::rotatePlane(float degree){
    if(glm::isnan(degree) || degree == 0) return;
//    cout << glm::degrees(degree) << endl;
    if(glm::abs(glm::degrees(degree)) < 1) return;
//    rotated_M = glm::rotate(glm::mat4(1.0f), degree, glm::vec3(0, 1, 0))*rotated_M;
//    rotated_M = glm::rotate(rotated_M, degree, glm::vec3(0, 1, 0));
    rotated_M = glm::rotate(glm::mat4(1), degree, glm::vec3(0, 1, 0));
//    update();
}
void Transform::switchToToneShading(){
    
    for(Node* child: children){
        child->switchToToneShading();
    }
}
