//
//  Turtle.cpp
//  PA0
//
//  Created by Karl Wang on 3/15/20.
//

#include "Turtle.hpp"
Turtle::Turtle(){
    currPos = glm::vec3(0);
    currAngle = 90.0;
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
void Turtle::drawLine(){
    
}
