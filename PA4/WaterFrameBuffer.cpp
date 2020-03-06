//
//  WaterFrameBuffer.cpp
//  PA0
//
//  Created by Karl Wang on 3/4/20.
//

#include "WaterFrameBuffer.hpp"
WaterFrameBuffer::WaterFrameBuffer(int width, int height){
    original_width = width;
    original_height = height;
    
    
    
    
    // create a frame buffer for reflection
    glGenFramebuffers(1, &reflectionFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    
    // create texture for reflection
    glGenTextures(1, &reflectionTexture);
    glBindTexture(GL_TEXTURE_2D, reflectionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, REFLECTION_WIDTH, REFLECTION_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reflectionTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectionTexture, 0);

    // create depth buffer for reflectoin
    glGenRenderbuffers(1, &reflectionDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, reflectionDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, REFLECTION_WIDTH, REFLECTION_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, reflectionDepthBuffer);
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    //unbind the buffer
    unbindFrameBuffer();


    
    
    
    
    // create a frame buffer for refraction
    glGenFramebuffers(1, &refractionFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    
    // create texture for refraction
    glGenTextures(1, &refractionTexture);
    glBindTexture(GL_TEXTURE_2D, refractionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, REFRACTION_WIDTH, REFRACTION_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, refractionTexture, 0);
    // create depth texture for refraction
    glGenTextures(1, &refractionDepthTexture);
    glBindTexture(GL_TEXTURE_2D, refractionDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, REFRACTION_WIDTH, REFRACTION_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, refractionDepthTexture, 0);
    //unbind current buffer
    unbindFrameBuffer();

}

void WaterFrameBuffer::unbindFrameBuffer(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
//    glViewport(0, 0, original_width, original_height);
}
void WaterFrameBuffer::bindReflectionFrameBuffer(){
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, reflectionFrameBuffer);
//    glViewport(0, 0, REFLECTION_WIDTH, REFLECTION_HEIGHT);
}
void WaterFrameBuffer::bindRefractionFrameBuffer(){
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, refractionFrameBuffer);
//    glViewport(0, 0, REFRACTION_WIDTH, REFRACTION_HEIGHT);
}
GLuint WaterFrameBuffer::getReflectionTexture(){
    return reflectionTexture;
}
GLuint WaterFrameBuffer::getRefractionTexture(){
    return refractionTexture;
}
