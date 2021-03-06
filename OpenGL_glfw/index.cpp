#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

glm::mat4 scale(glm::mat4 trans, glm::vec3 scale);
glm::mat4 rotate(glm::mat4 trans, float degree, glm::vec3 direction);
glm::mat4 translate(glm::mat4 trans, glm::vec3 move);

bool initWindow();
void initVBO();

GLFWwindow* window = NULL;

bool initWindow(){
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  #ifdef __APPLE__
  glfwWindowHint(GFLW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if(window == NULL){
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    std::cout << "Failed to initialize GLAD" << std::endl;
    return false;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  return true;
}

void initVBO(){
  float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
  };

  unsigned int indices[] = {
    0, 1, 3, 
    1, 2, 3
  };

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  /* int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;*/
}

int main (int argc, char* args[]){
  if(!initWindow()) return -1;
  
  initVBO();

  Shader ourShader("vertex.glsl", "fragment.glsl");

  glm::mat4 trans = glm::mat4(1.0f);
  trans = scale(trans, glm::vec3(0.15, 0.15, 0.15));
  trans = translate(trans, glm::vec3(3.0f, 3.0f, 0.0f));
  //trans = rotate(trans, 45.0f, glm::vec3(0.0, 0.0, 1.0));

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

  unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
  int modelLoc = glGetUniformLocation(ourShader.ID, "model");
  int viewLoc = glGetUniformLocation(ourShader.ID, "view");
  int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
  
  while(!glfwWindowShouldClose(window)){

    processInput(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader.use();

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window){
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

glm::mat4 scale(glm::mat4 trans, glm::vec3 scale) {
  return trans = glm::scale(trans, scale);
}

glm::mat4 rotate(glm::mat4 trans, float degree, glm::vec3 direction){
  return glm::rotate(trans, glm::radians(degree), direction);
}

glm::mat4 translate(glm::mat4 trans, glm::vec3 move) {
  return glm::translate(trans, move);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
  glViewport(0, 0, width, height);
}
