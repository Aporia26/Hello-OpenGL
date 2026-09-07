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

int main(){

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Tranforming SHit", NULL, NULL);
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
    Shader ourShader("C:/Users/Avita/Desktop/MY FOLDER/codingEXE/Practice_/opengl_learningBasics/src/shaders/transform.vs",
      "C:/Users/Avita/Desktop/MY FOLDER/codingEXE/Practice_/opengl_learningBasics/src/shaders/transform.fs");

    float vertices[] = {
      // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = {
      0,1,3,
      1,2,3
    };

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1,&VBO);
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&EBO);

      glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 1. Position Attribute (Location 0) - Size 3
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 2. Texture Coordinate Attribute (Location 1) - Size 2
    // Skips 3 position floats + 3 color floats = Starts at index 6
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(1);



    //texture1
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 

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
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load("C:/Users/Avita/Desktop/MY FOLDER/codingEXE/Practice_/opengl_learningBasics/ScaryTroll_tex.png", &imgwidth, &imgheight, &imgnrChannels, 3);

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


    //render loop
    while(!glfwWindowShouldClose(window)){

    processInput(window);

    
    glClearColor(0.2f, 0.2, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //scaling and rotating our shape
    glm::mat4 trans = glm::mat4(1.0f);
    //trans = glm::rotate(trans, glm::radians(90.f), glm::vec3(0.f,0.f,1.f));
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));  //rotating in real time
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

    // bind Texture
     glBindTexture(GL_TEXTURE_2D, texture);


    // get matrix's uniform location and set matrix
    ourShader.use();
    unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

   // render container
   glBindVertexArray(VAO);
   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

   glfwSwapBuffers(window);
   glfwPollEvents();
    

    }

     glDeleteVertexArrays(1, &VAO);
     glDeleteBuffers(1, &VBO);
     glDeleteBuffers(1, &EBO);

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