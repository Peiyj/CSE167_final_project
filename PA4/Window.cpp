#include "Window.h"

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid
 * declaring global or static variables.
 */
namespace
    {
    int width, height;
    std::string windowTitle("GLFW Starter Project");
    int state = 0;
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
    float far = 1000;
    glm::vec3 up(0.0, 1.0, 0.0); // The universal up direction of the camera.
    glm::mat4 view = glm::lookAt(cameraPos, center, up); // View matrix, defined by eye, center and up.
    glm::mat4 projection; // Projection matrix.
    
    
    GLuint program; // The shader program id.
    GLuint projectionLoc; // Location of projection in shader.
    GLuint viewLoc; // Location of view in shader.
    
    GLuint normal_program; // The shader program id.
    GLuint toon_program; // The shader program id.
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
    
    
    
    
    Terrain* terrain;

    // timing
    float deltaTime = 0.0f;    // time between current frame and last frame
    float lastFrame = 0.0f;

    };

bool Window::initializeProgram()
{
    // Create a shader program with a vertex shader and a fragment shader.
    program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    projectionLoc = glGetUniformLocation(program, "projection");
    viewLoc = glGetUniformLocation(program, "view");
    normal_program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    // Create a shader program with a vertex shader and a fragment shader.
    toon_program = LoadShaders("shaders/LightShader.vert", "shaders/LightShader.frag");

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


    n_projectionLoc = glGetUniformLocation(normal_program, "projection");
    n_viewLoc = glGetUniformLocation(normal_program, "view");
    
    dlightColor_loc = glGetUniformLocation(toon_program, "dlightColor");
    dlightDirection_loc = glGetUniformLocation(toon_program, "dlightDirection");
    cameraPos_loc = glGetUniformLocation(toon_program, "cameraPos");
    toon_projectionLoc = glGetUniformLocation(toon_program, "projection");
    toon_viewLoc = glGetUniformLocation(toon_program, "view");
    return true;
}

bool Window::initializeObjects()
{
    
    // Create a point cloud consisting of cube vertices.
    terrain = new Terrain(terrainProgram, 0, 0);

    teapot = new OBJObject("./teapot.obj", toon_program);
    
    // Create a directional light source
    dlight = new DirectionalLight(glm::vec3(1,1,0), glm::vec3(-1, -1, 0));


    return true;
}

void Window::cleanUp()
{
    // Deallcoate the objects.

    glDeleteProgram(normal_program);
    glDeleteProgram(toon_program);
}

GLFWwindow* Window::createWindow(int width, int height)
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
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);
    
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
    //    width = w;
    //    height = h;
    // Set the viewport size.
    glViewport(0, 0, width, height);
    // Set the projection matrix.
    projection = glm::perspective(glm::radians(fovy),
                                  (float)width / (float)height, near, far);
}

void Window::idleCallback()
{
    // Perform any updates as necessary.

}

void Window::displayCallback(GLFWwindow* window)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    glUseProgram(terrainProgram);
    glUniformMatrix4fv(terrainViewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(terrainProjectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    terrain->draw();
    glUseProgram(toon_program);
    glUniformMatrix4fv(toon_viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(toon_projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(dlightColor_loc, 1, glm::value_ptr(dlight->color));
    glUniform3fv(dlightDirection_loc, 1, glm::value_ptr(dlight->direction));
    glUniform3fv(cameraPos_loc, 1, glm::value_ptr(cameraPos));
    teapot->draw();
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

    float cameraSpeed = 10 * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void Window::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if(click){
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
            case GLFW_KEY_F1:
                // Set currentObj to cube
                
                break;
            case GLFW_KEY_F2:
                // Set currentObj to cubePoints
                
                break;
            case GLFW_KEY_F3:

                break;
                
            case GLFW_KEY_R:
            {
                view = glm::lookAt(cameraPos, center, cameraUp);
                break;
            }
            default:
                break;
        }
    }
}
