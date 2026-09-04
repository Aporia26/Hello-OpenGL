#include<glad/glad.h>        //Be sure to include GLAD(function location & pointer) before GLFW(os independance)
#include<GLFW/glfw3.h>
#include<iostream>
#include<cmath>

// ******************************************* WINDOW CREATION *************************************************************************

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE)){
    glfwSetWindowShouldClose(window,true);
  }
}

int main(){

    /********************************
    making the window for our project
    **********************************/
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

GLFWwindow* window = glfwCreateWindow(800, 600, "LearOpenGL", NULL, NULL);
  if(window == NULL){
    std::cout << "Failed to create GLFW window!" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
std::cout << "Failed to initialize GLAD" << std::endl;
return -1;
}

glViewport(0, 0, 800, 600);   //tells our rederin window size
glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //handles if user resizes the window

//render loop
while(!glfwWindowShouldClose(window)){

  /********************
    Handling User Inputs
   *********************/
  processInput(window);

  /**********************
   The rendering commands
  ***********************/
 glClearColor(0.2f, 0.2f, 0.2f, 1.f);
 glClear(GL_COLOR_BUFFER_BIT);
 
  //check and call events and swap buffers.
  glfwPollEvents();
  glfwSwapBuffers(window);
}

glfwTerminate();
    return 0;
}