#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "stb_image.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//Move and rotate control
float moveX, moveY, moveZ = 0.0f;
float rotateX, rotateY, rotateZ = 0.0f;
float angleX, angleY, angleZ = 0.0f;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //For resizing the window for any size the user wants
}

void processInput(GLFWwindow* window)   //
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); //When ESC pressed, window should close
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {

            moveY += 0.01f;
        
    }
    else if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) && moveY >= 0.0f) {
        moveY -= 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        moveZ -= 0.001f;
        if (angleZ < 0.5f)
            angleZ -= 0.001f;
        else if (angleX == 0.0f)
            angleZ = 0.5f;
        else
            angleZ += 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        moveZ += 0.001f;
        if (angleZ < 0.5f)
            angleZ += 0.001f;
        else if (angleX == 0.0f)
            angleZ = 0.5f;
        else
            angleZ -= 0.001f;

    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        moveX += 0.001f;
        if (angleX < 0.5f)
            angleX -= 0.001f;
        else if (angleX == 0.0f)
            angleX = 0.5f;
        else
            angleX += 0.001f;

    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        moveX -= 0.001f;
        if (angleX < 0.5f)
            angleX += 0.001f;
        else if (angleX == 0.0f)
            angleX = 0.0f;
        else
            angleX -= 0.001f;

    }
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
int main() {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //We tell that the major and minor version are v3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    //First argument is the type of the buffer we want to copy data into
    //the vertex buffer object currently bound to the GL_ARRAY_BUFFER target. 
    //The second argument specifies the size of the data (in bytes) we want to pass to the buffer; 
    //a simple sizeof of the vertex data suffices. 
    //The third parameter is the actual data we want to send.
    //The fourth parameter specifies how we want the graphics card to manage the given data.This can take 3 forms:
        //GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times. 
        //GL_STATIC_DRAW : the data is set only once and used many times.
        //GL_DYNAMIC_DRAW : the data is changed a lot and used many times.


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rolling Ball 3D", NULL, NULL); //Creating a window with size 800x600px
    if (window == NULL) //If the window have not opened
    {
        std::cout << "Failed to create GLFW window" << std::endl;  
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))    //GLAD load check
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST); //We should enable the depth test from GLFW library, if we want to use the z-buffer (depth buffer)
    //Without a z-buffer, since OpenGL draws your cube with triangles, newly created triangles could be created on top of each other
    glDepthFunc(GL_LESS);
    Shader shader("./shaders/modelLoading.vert", "./shaders/modelLoading.frag");

    // load models
    // -----------
    Model ourModel("./models/beach-ball/beachBall.obj");


    while (!glfwWindowShouldClose(window))  //glfwWindowShouldClose checks if GLFW told to close.
    {

        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //input
        processInput(window);

        //rendering
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);   //At the start of frame we want to clear the screen. 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Otherwise we would still see the results from the previous frame
                                                //glClearColor: The color to be filled with glClear
                                                //glClear: We pass in buffer bits to specify which buffer we would like to clear.
                                                //We want to clear color and depth buffers before each frame is created
     
        // don't forget to enable shader before setting uniforms
        shader.use();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f); 
        //camera.Zoom: Adjusts the Field of View (FoV) 
        glm::mat4 view = camera.GetViewMatrix();
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        
        


        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(moveX, moveY, moveZ));

        model = glm::rotate(model, angleZ, glm::vec3(0.5f, rotateY, rotateZ)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, angleX, glm::vec3(rotateX, rotateY, 0.50f)); // translate it down so it's at the center of the scene
        
        model = glm::scale(model, glm::vec3(0.03f, 0.03f, 0.03f));	// it's a bit too big for our scene, so scale it down
        
        shader.setMat4("model", model);
        ourModel.Draw(shader);



        //check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }



    glfwTerminate(); //As soon as the project finished we clean/delete all of the GLFW's resources
	return 0;
}