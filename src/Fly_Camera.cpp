#include<glad/glad.h>        //Be sure to include GLAD(function location & pointer) before GLFW(os independance)
#include<GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "shaders/shader.h"

#include<iostream>
#include<cmath>

void FrameBuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
//MOUSE INPUTS
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//CAMERA VARIABLES
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	//yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;   //rotate in right/left and no rolling ie z in our need.
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

//MOVEMENT SPEED
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main(){

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Fly Camera & INputs", NULL, NULL);
  if(window == NULL){
    std::cout << "Failed to initialize GLFW Window!\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, FrameBuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // TELL GLFW to CAPTURE our MOUSE
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback); 


    //shader
    Shader ourShader("src/shaders/coord_sys.vs", "src/shaders/coord_sys.fs");

    float vertices[] = {
    // Positions          // Texture Coords (Mapped cleanly per face)
    
    // Back Face (Width: 0.7, Height: 1.0)
    -0.35f, -0.5f, -0.35f,  0.0f, 0.0f, // bottom-left
     0.35f, -0.5f, -0.35f,  1.0f, 0.0f, // bottom-right
     0.35f,  0.5f, -0.35f,  1.0f, 1.0f, // top-right
     0.35f,  0.5f, -0.35f,  1.0f, 1.0f, // top-right
    -0.35f,  0.5f, -0.35f,  0.0f, 1.0f, // top-left
    -0.35f, -0.5f, -0.35f,  0.0f, 0.0f, // bottom-left

    // Front Face (Width: 0.7, Height: 1.0)
    -0.35f, -0.5f,  0.35f,  0.0f, 0.0f,
     0.35f, -0.5f,  0.35f,  1.0f, 0.0f,
     0.35f,  0.5f,  0.35f,  1.0f, 1.0f,
     0.35f,  0.5f,  0.35f,  1.0f, 1.0f,
    -0.35f,  0.5f,  0.35f,  0.0f, 1.0f,
    -0.35f, -0.5f,  0.35f,  0.0f, 0.0f,

    // Left Face (Depth: 0.7, Height: 1.0)
    -0.35f,  0.5f,  0.35f,  1.0f, 1.0f,
    -0.35f,  0.5f, -0.35f,  0.0f, 1.0f,
    -0.35f, -0.5f, -0.35f,  0.0f, 0.0f,
    -0.35f, -0.5f, -0.35f,  0.0f, 0.0f,
    -0.35f, -0.5f,  0.35f,  1.0f, 0.0f,
    -0.35f,  0.5f,  0.35f,  1.0f, 1.0f,

    // Right Face (Depth: 0.7, Height: 1.0)
     0.35f,  0.5f,  0.35f,  0.0f, 1.0f,
     0.35f,  0.5f, -0.35f,  1.0f, 1.0f,
     0.35f, -0.5f, -0.35f,  1.0f, 0.0f,
     0.35f, -0.5f, -0.35f,  1.0f, 0.0f,
     0.35f, -0.5f,  0.35f,  0.0f, 0.0f,
     0.35f,  0.5f,  0.35f,  0.0f, 1.0f,

    // Bottom Face (Width: 0.7, Depth: 0.7) - Perfectly Square caps
    -0.35f, -0.5f, -0.35f,  0.0f, 1.0f,
     0.35f, -0.5f, -0.35f,  1.0f, 1.0f,
     0.35f, -0.5f,  0.35f,  1.0f, 0.0f,
     0.35f, -0.5f,  0.35f,  1.0f, 0.0f,
    -0.35f, -0.5f,  0.35f,  0.0f, 0.0f,
    -0.35f, -0.5f, -0.35f,  0.0f, 1.0f,

    // Top Face (Width: 0.7, Depth: 0.7) - Perfectly Square caps
    -0.35f,  0.5f, -0.35f,  0.0f, 1.0f,
     0.35f,  0.5f, -0.35f,  1.0f, 1.0f,
     0.35f,  0.5f,  0.35f,  1.0f, 0.0f,
     0.35f,  0.5f,  0.35f,  1.0f, 0.0f,
    -0.35f,  0.5f,  0.35f,  0.0f, 0.0f,
    -0.35f,  0.5f, -0.35f,  0.0f, 1.0f
};

//Add many such cubes into the space!!
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

    unsigned int VBO, VAO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
   
      glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 1. Position Attribute (Location 0) - Size 3
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// 2. Texture Coordinate Attribute (Location 1) - Size 2 -> CHANGED FROM 6 to 3!
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);


    //texture1
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); 

    // Safety check: tell OpenGL to read pixels byte-by-byte (fixes weird dimensions)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int imgwidth, imgheight, imgnrChannels;
    unsigned char *data;
  data = stbi_load("doorTex.jpg", &imgwidth, &imgheight, &imgnrChannels, 3);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgwidth, imgheight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
        //texture2
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); 

    // Safety check: tell OpenGL to read pixels byte-by-byte (fixes weird dimensions)
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    data = stbi_load("ScaryTroll_tex.png", &imgwidth, &imgheight, &imgnrChannels, 4);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgwidth, imgheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    ourShader.use(); 
ourShader.setInt("texture1", 0); // Assigns uniform sampler2D texture1 to GL_TEXTURE0
ourShader.setInt("texture2", 1); // Assigns uniform sampler2D texture2 to GL_TEXTURE1

    //render loop
    while(!glfwWindowShouldClose(window)){

    processInput(window);

    //CHANGE DELTA TIME FOR MOVEMENT SPEED
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame; 

    glClearColor(0.2f, 0.2, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();


      //CAMERA vectors----->
    //direction(target)
   // glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);
    //glm::vec3 cameraTarget = glm::vec3(0.0f,0.0f,0.0f);
    //glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    //right axis
    //glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
    //glm::vec3 cameraRight = glm::normalize(glm::cross(up,cameraDirection));
    //up axis
    //glm::vec3 cameraUp = glm::cross(cameraDirection,cameraRight);
    //making our lookAt matrix to tranform our camera--->Example
   // glm::mat4 View;
   // View = glm::lookAt(glm::vec3(0.0f,0.0f,3.0f),  //needs positon
   //                    glm::vec3(0.0f, 0.0f, 0.0f), //target
  // 	                  glm::vec3(0.0f, 1.0f, 0.0f));  //and up      


      
    // create transformations
     // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);     

        //THE LOOKAT FUNCTION (MATRIX)
        view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
        ourShader.setMat4("view", view);
      //lookat making for camera but in funky way roataing camera in predefined radius
     // const float radius = 15.0f;
      //float camX = sin(glfwGetTime()) * radius;
      //float camZ = cos(glfwGetTime()) * radius;
      //view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));


        
        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

        glEnable(GL_DEPTH_TEST);        
 
   //to render many cubes--->
  glBindVertexArray(VAO);
   for(unsigned int i = 0; i < 10; i++)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * i;                                //giving each a random orientation
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    ourShader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

   glfwSwapBuffers(window);
   glfwPollEvents();
    
    }

     glDeleteVertexArrays(1, &VAO);
     glDeleteBuffers(1, &VBO);

    glfwTerminate();

  return 0;
}
void FrameBuffer_size_callback(GLFWwindow* window, int width, int height){
   glViewport(0, 0, width, height);
}

//TO WALK AROUND ADD MORE INPUT KEYS
void processInput(GLFWwindow* window){
if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
     glfwSetWindowShouldClose(window, true); //  Cleaner exit
  }

 // const float cameraSpeed = 0.05f; // adjust accordingly since it's system dependant rn.
 const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.4f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
//ZOOMING
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}
