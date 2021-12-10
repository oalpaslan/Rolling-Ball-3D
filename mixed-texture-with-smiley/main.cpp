#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); //For resizing the window for any size the user wants
}

void processInput(GLFWwindow* window)   //
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); //When ESC pressed, window should close
}

int main() {
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  //We tell that the major and minor version are v3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))    //GLAD load check
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

 /*   glViewport(0, 0, 800, 600);  //For telling OpenGL the size of rendering window.
                                        //First two parameters are for setting the location of the lower left corner of the window
                                        //3rd and 4th parameters set width and height of the rendering window (in pixels) */

    
    //We do have to tell GLFW we want to call size callback function on every window resize by registering it

    Shader ourShader("./shaders/vertex.vert", "./shaders/fragment.frag");



    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };


    unsigned int VBO, VAO, EBO; //VBO: Vertex Buffer Object, VAO: Vertex Array Object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Copy our vertices array in a buffer for OpenGL to use

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);          //Takes vertices as parameter
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    //Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //The first parameter specifies which vertex attribute we want to configure. 
        //We specified the location of the position vertex attribute in the vertex shader with "layout (location = 0)"[ln.8]
        //This sets the location of the vertex attribute to 0 and since we want to pass data to this vertex attribute, we pass in 0.
    //2nd: Size of the vertex attribute. The vertex attribute is a vec3 [ln.8] so it is composed of 3 values.
    //3rd: Type of the data which is GL_FLOAT 
    //4th: If we want the data to be normalized. 
        //If we're inputting integer data types (int, byte) and we've set this to GL_TRUE, the integer data is normalized to 0 
        //(or -1 for signed data) and 1 when converted to float. Not relevant
    //5th: Known as "stride". The space between consecutive vertex attributes.
        //Since the next set of position data is located exactly 3 times the size of a float away we specify that value as the stride.
    
    //Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); //We can say that since we set the location to 1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    unsigned int texture, texture2;
    glGenTextures(1, &texture); //1st: How many textures we want to generate, 2nd: store them in an array
    glBindTexture(GL_TEXTURE_2D, texture); 


    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("./textures/container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data); //Free the image memory
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("./textures/awesomeface.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use(); // don't forget to activate/use the shader before setting uniforms!
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // or set it via the texture class
    ourShader.setInt("texture2", 1);

    while (!glfwWindowShouldClose(window))  //glfwWindowShouldClose checks if GLFW told to close.
    {
        //input
        processInput(window);

        //rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   //At the start of frame we want to clear the screen. 
        glClear(GL_COLOR_BUFFER_BIT);           //Otherwise we would still see the results from the previous frame
                                                //glClearColor: The color to be filled with glClear
                                                //glClear: Wee pass in buffer bits to specify which buffer we would like to clear.

        
        
        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        

        
        ourShader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


    glfwTerminate(); //As soon as the project finished we clean/delete all of the GLFW's resources
	return 0;
}