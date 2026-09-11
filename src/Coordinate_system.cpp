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

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(){

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Tranforming SHit", NULL, NULL);
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
    //shader
    Shader ourShader("C:/Users/Avita/Desktop/MY FOLDER/codingEXE/Practice_/opengl_learningBasics/src/shaders/coord_sys.vs",
      "C:/Users/Avita/Desktop/MY FOLDER/codingEXE/Practice_/opengl_learningBasics/src/shaders/coord_sys.fs");

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
/*glm::vec3 cubePositions[] = {
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
};*/

    //unsigned int VBO, VAO, EBO;
    unsigned int VBO, VAO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    //glGenBuffers(1,&EBO);

      glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
   // stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
  data = stbi_load("C:/Users/Avita/Desktop/MY FOLDER/codingEXE/Practice_/opengl_learningBasics/doorTex.jpg", &imgwidth, &imgheight, &imgnrChannels, 3);

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
    data = stbi_load("C:/Users/Avita/Desktop/MY FOLDER/codingEXE/Practice_/opengl_learningBasics/ScaryTroll_tex.png", &imgwidth, &imgheight, &imgnrChannels, 4);

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


    ourShader.use(); // Make sure your custom Shader class has a use() or activate() method!
ourShader.setInt("texture1", 0); // Assigns uniform sampler2D texture1 to GL_TEXTURE0
ourShader.setInt("texture2", 1); // Assigns uniform sampler2D texture2 to GL_TEXTURE1



    //render loop
    while(!glfwWindowShouldClose(window)){

    processInput(window);

    
    glClearColor(0.2f, 0.2, 0.3f, 1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();
        

    // create transformations
        glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        //model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        //rotate in real time-->
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // retrieve the matrix uniform locations
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(ourShader.ID, "view");
        // pass them to the shaders (3 different ways)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("projection", projection);

 //Z-Buffer helps in rendering as per depht(instead of overwritten not rendering thing ones done even if above it)
        glEnable(GL_DEPTH_TEST);        
 
   // render container
   glBindVertexArray(VAO);
   //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
   glDrawArrays(GL_TRIANGLES,0, 36); // we need 36 vertices for a cube instead of plane!!

   //to render many cubes--->
  /* glBindVertexArray(VAO);
   for(unsigned int i = 0; i < 10; i++)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * i; 
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    ourShader.setMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}
*/
   glfwSwapBuffers(window);
   glfwPollEvents();
    

    }

     glDeleteVertexArrays(1, &VAO);
     glDeleteBuffers(1, &VBO);
     //glDeleteBuffers(1, &EBO);

    glfwTerminate();

  return 0;
}
void FrameBuffer_size_callback(GLFWwindow* window, int width, int height){
   glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
     glfwSetWindowShouldClose(window, true); //  Cleaner exit
  }
}