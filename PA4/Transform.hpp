//
//  Transform.hpp
//  PA0
//
//  Created by Karl Wang on 2/20/20.
//

#ifndef Transform_hpp
#define Transform_hpp

#include <stdio.h>
#include "Node.hpp"
class Transform: public Node{
private:
    std::vector<Node*> children;
    glm::mat4 M;
    float degree;
    int forward;
    glm::mat4 moved_M;
    glm::mat4 original_M;
    glm::mat4 scaled_M;
    glm::mat4 rotated_M;
    bool walking;
    bool moving;
public:
    Transform(const glm::mat4& M);
    ~Transform();
    void draw(const glm::mat4& C);
    void update();
    void moveRotation();
    void addChild(Node*);
    void removeChild(Node*);
    void startWalking(int forward);
    void rotateMatrix(float degree);
    void scaleMatrix(float amount);
    void mouseRotate(float, glm::vec3);
    void moveMatrix(glm::vec3&);
    void updateRotation();
    
    void rotatePlane(float degree);
    void lookAt(glm::vec3& pos, glm::vec3& prevPos);
    void switchToToneShading();
    
};
#endif /* Transform_hpp */
