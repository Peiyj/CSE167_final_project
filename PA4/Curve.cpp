//
//  Curve.cpp
//  PA0
//
//  Created by Karl Wang on 2/21/20.
//

#include "Curve.hpp"
#include <iostream>
using namespace std;

Curve::Curve(GLuint program, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3){
    modelLoc = glGetUniformLocation(program, "model");
    colorLoc = glGetUniformLocation(program, "color");
    color = glm::vec3(1, 0, 0.2);
//    int numSeg = 150;
    
    a = -p0+3.0f*p1-3.0f*p2+p3;
    b = 3.0f*p0-6.0f*p1+3.0f*p2;
    c = -3.0f*p0 + 3.0f*p1;
    d = p0;
    
//    for(float i = 0; i <= numSeg; i++){
//        float t = i/numSeg;
//        lines.push_back(calcBezier(t));
//    }
    
    
    
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the vertices.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lines.size(),
        lines.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access vertices through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    // Unbind from the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}
Curve::~Curve(){
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}


glm::vec3 Curve::calcBezier(float t){
    glm::vec3 res = a*t*t*t+b*t*t+c*t+d;
    return res;
}
void Curve::draw(){
    glBindVertexArray(vao);
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    glUniformMatrix4fv(colorLoc, 1, GL_FALSE, glm::value_ptr(color));
    

    // Draw triangles using the indices in the second VBO, which is an
    // elemnt array buffer.
    glLineWidth(10.0f);
    
    glDrawArrays(GL_LINE_STRIP, 0, lines.size());
    glBindVertexArray(0);

}

