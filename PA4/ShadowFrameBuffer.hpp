//
//  WaterFrameBuffer.hpp
//  PA0
//
//  Created by Karl Wang on 3/4/20.
//

#ifndef ShadowFrameBuffer_hpp
#define ShadowFrameBuffer_hpp

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
class ShadowFrameBuffer{
private:
    int original_width;
    int original_height;
    GLuint reflectionFrameBuffer;
    GLuint reflectionTexture;
    GLuint reflectionDepthBuffer;
    
    GLuint refractionFrameBuffer;
    GLuint refractionTexture;
    GLuint refractionDepthTexture;
public:
    ShadowFrameBuffer(int width, int height);
    void unbindFrameBuffer();
    
};


#endif /* WaterFrameBuffer_hpp */
