#include "Window.h"

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid
 * declaring global or static variables.
 */
using namespace std;
namespace
    {
    int width, height;
    std::string windowTitle("GLFW Starter Project");
    bool click = false;
    
    // camera
    glm::vec3 lastRot;
    float yaw   = -90.0f;    // yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
    float pitch =  0.0f;
    float lastX =  800.0f / 2.0;
    float lastY =  600.0 / 2.0;
    
    glm::vec3 cameraPos(0, 0, 20); // Camera position.
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); //The front direction
    glm::vec3 center(0, 0, 0); // The point we are looking at.
    glm::vec3 cameraUp(0, 1, 0); // The up direction of the camera.
    float fovy = 60;
    float near = 1;
    float far = 5000;
    glm::vec3 up(0.0, 1.0, 0.0); // The universal up direction of the camera.
    glm::mat4 view = glm::lookAt(cameraPos, center, up); // View matrix, defined by eye, center and up.
    glm::mat4 projection; // Projection matrix.
    
    
    GLuint program; // The shader program id.
    GLuint projectionLoc; // Location of projection in shader.
    GLuint viewLoc; // Location of view in shader.
    
    GLuint normal_program; // The shader program id.
    GLuint toon_program; // The shader program id.
    GLuint depth_program; //The depthMap program id.
    GLuint depth_lightLoc; //The light matrix loc of the depthMap
    GLuint depth_modelLoc; //The model matrix loc of the depthMap
    GLuint n_projectionLoc; // Location of projection in shader.
    GLuint n_viewLoc; // Location of view in shader.
    GLuint toon_projectionLoc; // Location of projection in shader.
    GLuint toon_viewLoc; // Location of view in shader.
    GLuint dlightColor_loc; //location of the directional light color
    GLuint dlightDirection_loc; //location of the directional light direction
    GLuint cameraPos_loc; // camera location

    OBJObject* teapot;
    DirectionalLight* dlight;
    
    GLuint terrainProgram;
    GLuint terrainProjectionLoc;
    GLuint terrainViewLoc;
    
    Water* water;
    GLuint waterPorgram;
    GLuint waterProjectionLoc;
    GLuint waterViewLoc;
    
    
    
    GLuint skyboxProgram;
    Cube* skybox;
    
    
    
    
    ShadowFrameBuffer* shadowFrameBuffer;
    
    WaterFrameBuffer* waterFrameBuffer;
    
    
    Terrain* terrain;
    Terrain* terrain_ds;

    // timing
    float deltaTime = 0.0f;    // time between current frame and last frame
    float lastFrame = 0.0f;
    
    // enable movement
    bool enableMovement = false;
    };

bool Window::initializeProgram()
{
    // Create a shader program with a vertex shader and a fragment shader.
    projectionLoc = glGetUniformLocation(program, "projection");
    viewLoc = glGetUniformLocation(program, "view");
    normal_program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    // Create a shader program with a vertex shader and a fragment shader.
    toon_program = LoadShaders("shaders/LightShader.vert", "shaders/LightShader.frag");
    // Create a shader program that tests the depth of the object (first pass of shadow mapping)
    depth_program = LoadShaders("shaders/depthMap.vert", "shaders/depthMap.frag");
    // location of projection and view
    projectionLoc = glGetUniformLocation(program, "projection");
    viewLoc = glGetUniformLocation(program, "view");
    // Check the shader program.
    if (!normal_program)
    {
        std::cerr << "Failed to initialize normal shader program" << std::endl;
        return false;
    }
    
    // Check the shader program.
    if (!toon_program)
    {
        std::cerr << "Failed to initialize toon shader program" << std::endl;
        return false;
    }
    
    terrainProgram = LoadShaders("shaders/TerrainShader.vert", "shaders/TerrainShader.frag");
    terrainProjectionLoc = glGetUniformLocation(terrainProgram, "projection");
    terrainViewLoc = glGetUniformLocation(terrainProgram, "view");
    
    waterPorgram = LoadShaders("shaders/WaterShader.vert", "shaders/WaterShader.frag");
    waterProjectionLoc = glGetUniformLocation(waterPorgram, "projection");
    waterViewLoc = glGetUniformLocation(waterPorgram, "view");
    

    n_projectionLoc = glGetUniformLocation(normal_program, "projection");
    n_viewLoc = glGetUniformLocation(normal_program, "view");
    
    dlightColor_loc = glGetUniformLocation(toon_program, "dlightColor");
    dlightDirection_loc = glGetUniformLocation(toon_program, "dlightDirection");
    cameraPos_loc = glGetUniformLocation(toon_program, "cameraPos");
    toon_projectionLoc = glGetUniformLocation(toon_program, "projection");
    toon_viewLoc = glGetUniformLocation(toon_program, "view");
    
    
    
    
    
    
    skyboxProgram = LoadShaders("shaders/SkyboxShader.vert", "shaders/SkyboxShader.frag");
    
    depth_lightLoc = glGetUniformLocation(depth_program, "lightSpaceMatrix");
    depth_modelLoc = glGetUniformLocation(depth_program, "model");
    return true;
}

bool Window::initializeObjects()
{

    // Create a point cloud consisting of cube vertices.
//    terrain = new Terrain(terrainProgram, 1, 0, false);

    teapot = new OBJObject("./teapot.obj", depth_program);
    
    // Create a directional light source
    //first is color, second is direction
//    dlight = new DirectionalLight(glm::vec3(1,1,0), glm::vec3(1, -1, 0));
    dlight = new DirectionalLight(glm::vec3(1,1,1), glm::vec3(1, -1, 1));
    
    terrain_ds = new Terrain(terrainProgram, 0, 0, true);
    
    water = new Water(waterPorgram, terrain_ds->getSize(),terrain_ds->getMiny(),
                      terrain_ds->getMaxy(), terrain_ds->getModel());
    
    waterFrameBuffer = new WaterFrameBuffer(width, height);
    water->setTexID(waterFrameBuffer->getReflectionTexture(), waterFrameBuffer->getRefractionTexture()
                    ,waterFrameBuffer->getRefractionDepthTexture());

    
    skybox = new Cube(4096, skyboxProgram);

    
    shadowFrameBuffer = new ShadowFrameBuffer(width, height);
    

    return true;
}

void Window::cleanUp()
{
    // Deallcoate the objects.

    glDeleteProgram(normal_program);
    glDeleteProgram(toon_program);
    glDeleteProgram(program);
    glDeleteProgram(waterPorgram);
    glDeleteProgram(terrainProgram);
    glDeleteProgram(skyboxProgram);
}

GLFWwindow* Window::createWindow(int w, int h)
{
    // Initialize GLFW.
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }
    
    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);
    
#ifdef __APPLE__ 
    // Apple implements its own version of OpenGL and requires special treatments
    // to make it uses modern OpenGL.
    
    // Ensure that minimum OpenGL version is 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Enable forward compatibility and allow a modern OpenGL context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(w, h, windowTitle.c_str(), NULL, NULL);
//    glfwGetFramebufferSize(window, &width, &height);
    // Check if the window could not be created.
    if (!window)
    {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }
    
    // Make the context of the window.
    glfwMakeContextCurrent(window);
    
#ifndef __APPLE__
    // On Windows and Linux, we need GLEW to provide modern OpenGL functionality.
    
    // Initialize GLEW.
    if (glewInit())
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return NULL;
    }
#endif
    
    // Set swap interval to 1.
    glfwSwapInterval(0);
    // Initialize the quad that will be textured with your image
    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);
    
    return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
    // In case your Mac has a retina display.
    glfwGetFramebufferSize(window, &width, &height);
#endif
    // Set the viewport size.
    glViewport(0, 0, width, height);
    // Set the projection matrix.
    projection = glm::perspective(glm::radians(fovy),
                                  (float)width / (float)height, near, far);
}

void Window::idleCallback()
{
    // Perform any updates as necessary.

    waterFrameBuffer->setWidthHeight(width, height);
    water->update();
}

void Window::displayCallback(GLFWwindow* window)
{
    
    // location of the light
    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // input
    // -----
    processInput(window);
    
    
    
    
    
    

    
    // Clear the color and depth buffers.
    glEnable(GL_CLIP_DISTANCE0);
    
    
    
    ///*
    // first redner to reflection FBO
    waterFrameBuffer->bindReflectionFrameBuffer();
    // move the camera below the water
    float distance = 2*(cameraPos.y-water->getHeight());
    cameraPos.y -= distance;
    pitch = -pitch;
    glm::mat4 reflectView = getViewMatrix();
    
    
    
    // clear the color bit
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // create the reflection plane
    glm::vec4 reflectionPlane = glm::vec4(0, 1, 0, -water->getHeight());
    // render the scene
    glUseProgram(terrainProgram);
    glUniform4fv(glGetUniformLocation(terrainProgram, "plane"), 1, glm::value_ptr(reflectionPlane));
    glUniformMatrix4fv(terrainViewLoc, 1, GL_FALSE, glm::value_ptr(reflectView));
    glUniformMatrix4fv(terrainProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
//        terrain->draw();
    glm::vec3 ambient = glm::vec3(0.4);
    glm::vec3 specular = glm::vec3(1);
    glUniform3fv(glGetUniformLocation(terrainProgram, "dirLight.ambient"), 1, glm::value_ptr(ambient));
    glUniform3fv(glGetUniformLocation(terrainProgram, "dirLight.diffuse"), 1, glm::value_ptr(dlight->color));
    glUniform3fv(glGetUniformLocation(terrainProgram, "dirLight.specular"), 1, glm::value_ptr(specular));
    glUniform3fv(glGetUniformLocation(terrainProgram, "dirLight.direction"), 1, glm::value_ptr(dlight->direction));
    glUniform3fv(glGetUniformLocation(terrainProgram, "viewPos"), 1, glm::value_ptr(cameraPos));
    terrain_ds->draw();
    
    
    
    glUseProgram(skyboxProgram);
    glUniform4fv(glGetUniformLocation(skyboxProgram, "plane"), 1, glm::value_ptr(reflectionPlane));
    glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_FALSE,
                       glm::value_ptr(reflectView));
    glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));
    skybox->draw();
    
    
    
    
    waterFrameBuffer->unbindFrameBuffer();

    
    cameraPos.y +=distance;
    pitch = -pitch;
    
//   */
    
//    glm::vec3 ambient = glm::vec3(0.4);
//    glm::vec3 specular = glm::vec3(1);

    

    // render again in the refraction fbo
    waterFrameBuffer->bindRefractionFrameBuffer();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
    glm::vec4 refractionPlane = glm::vec4(0, -1, 0, water->getHeight()+10.0f);
    glUseProgram(terrainProgram);
    glUniform4fv(glGetUniformLocation(terrainProgram, "plane"), 1, glm::value_ptr(refractionPlane));
    glUniformMatrix4fv(terrainViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(terrainProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
//    glUniform3fv(glGetUniformLocation(terrainProgram, "dirLight.ambient"), 1, glm::value_ptr(ambient));
//    glUniform3fv(glGetUniformLocation(terrainProgram, "dirLight.diffuse"), 1, glm::value_ptr(dlight->color));
//    glUniform3fv(glGetUniformLocation(terrainProgram, "dirLight.specular"), 1, glm::value_ptr(specular));
//    glUniform3fv(glGetUniformLocation(terrainProgram, "dirLight.direction"), 1, glm::value_ptr(dlight->direction));
    glUniform3fv(glGetUniformLocation(terrainProgram, "viewPos"), 1, glm::value_ptr(cameraPos));
    //        terrain->draw();
    terrain_ds->draw();
    
    
    
    glUseProgram(skyboxProgram);
    glUniform4fv(glGetUniformLocation(skyboxProgram, "plane"), 1, glm::value_ptr(refractionPlane));
    glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_FALSE,
                       glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));
    skybox->draw();
    waterFrameBuffer->unbindFrameBuffer();
    
    
    
    
    
    //render the scene normally
    
    glDisable(GL_CLIP_DISTANCE0);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(terrainProgram);
    glUniform4fv(glGetUniformLocation(terrainProgram, "plane"), 1, glm::value_ptr(glm::vec4(0)));
    glUniformMatrix4fv(terrainViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(terrainProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    
    
//    glUniform3fv(glGetUniformLocation(terrainProgram, "dirLight.ambient"), 1, glm::value_ptr(ambient));
//    glUniform3fv(glGetUniformLocation(terrainProgram, "dirLight.diffuse"), 1, glm::value_ptr(dlight->color));
//    glUniform3fv(glGetUniformLocation(terrainProgram, "dirLight.specular"), 1, glm::value_ptr(specular));
//    glUniform3fv(glGetUniformLocation(terrainProgram, "dirLight.direction"), 1, glm::value_ptr(dlight->direction));
    glUniform3fv(glGetUniformLocation(terrainProgram, "viewPos"), 1, glm::value_ptr(cameraPos));
    terrain_ds->draw();

    
    
    
    
    
    
    
    
    
    
    
    
    
    

    glUseProgram(waterPorgram);
    glUniform3fv(glGetUniformLocation(waterPorgram, "cameraPos"), 1, glm::value_ptr(cameraPos));
    glUniformMatrix4fv(waterProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(waterViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(glGetUniformLocation(waterPorgram, "dirLight.ambient"), 1, glm::value_ptr(ambient));
    glUniform3fv(glGetUniformLocation(waterPorgram, "dirLight.diffuse"), 1, glm::value_ptr(dlight->color));
    glUniform3fv(glGetUniformLocation(waterPorgram, "dirLight.specular"), 1, glm::value_ptr(specular));
    glUniform3fv(glGetUniformLocation(waterPorgram, "dirLight.direction"), 1, glm::value_ptr(dlight->direction));
    glUniform3fv(glGetUniformLocation(waterPorgram, "viewPos"), 1, glm::value_ptr(cameraPos));
    water->draw();
    

    glUseProgram(skyboxProgram);
    glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_FALSE,
                       glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projection"), 1, GL_FALSE,
                       glm::value_ptr(projection));
    
    skybox->draw();
    

    
    
    // set the function for mouse click
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // set the function to acquire cursor position
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
    
}
void Window::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = 110 * deltaTime;
    
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        water->moveWater(0.2);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        water->moveWater(-0.2);
    if (enableMovement == true){
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if(click && enableMovement){
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.5;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw   += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f)
        pitch = 89.0f;
        if(pitch < -89.0f)
        pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(direction);
    }
}

glm::vec3 Window::trackBallMapping(double xpos, double ypos) {
    glm::vec3 v;
    float d;
    v.x = (2.0f * xpos - width) / width;
    v.y = (height - 2.0f * ypos) / height;
    
    v.z = 0.0;
    
    d = glm::length(v);
    d = (d < 1.0f)?d:1.0f;
    v.z = glm::sqrt(1.001 - d * d);
    v = glm::normalize(v);
    return v;
    
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    //once we click the left button
    double x_rot, y_rot;
    switch (action){
        case GLFW_PRESS:{
            click = true;
            glfwGetCursorPos(window, &x_rot, &y_rot);
            lastX = x_rot;
            lastY = y_rot;
            lastRot = trackBallMapping(x_rot, y_rot);
            break;
        }
        case GLFW_RELEASE:{
            click = false;
            break;
        }
        default:
            break;
    }
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Check for a key press.
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
                
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            //switch to debug window drawline
            case GLFW_KEY_1:
                water->switchToDrawLine();
                break;
            case GLFW_KEY_2:
                water->switchToNormal();
                break;
            case GLFW_KEY_F3:

                break;
                
            case GLFW_KEY_T:
            {
                terrain_ds->switchToToneShading();
                break;
            }
            case GLFW_KEY_C:
            {
                if (enableMovement == true)
                    enableMovement = false;
                else
                    enableMovement = true;
                break;
            }
            case GLFW_KEY_N:
                delete(terrain_ds);
                terrain_ds = new Terrain(terrainProgram, 0, 0, true);
                break;
            default:
                break;
        }
    }
}
glm::mat4 Window::getViewMatrix(){
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    glm::mat4 res = glm::lookAt(cameraPos, cameraPos + front, cameraUp);
    return res;
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
