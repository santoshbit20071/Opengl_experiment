#include <iostream>
#include <cassert>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
       glfwSetWindowShouldClose(window, GL_TRUE);
}

bool isCompileSuccess(GLuint shader) {
  GLint success;
  GLchar info[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, info);
    std::cout << "SHADER COMIPLE ERROR : " << info << std::endl;
    return false;
  }
  return true;
}

bool isLinkSuccess(GLuint program) {
  GLint success;
  GLchar info[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, info);
    std::cout << "PROGRAM LINK ERROR : " << info << std::endl;
    return false;
  }
  return true;
}

GLuint CreateAndAttachShaderProgram() {

  const GLchar* vertex_shader_source = "#version 330 core\n"
        "layout (location = 0) in vec3 position;\n"
            "void main()\n"
                "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\0";
  const GLchar* fragment_shader_source = "#version 330 core\n"
        "out vec4 color;\n"
            "void main()\n"
                "{\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

  GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexshader, 1, &vertex_shader_source, NULL);
  glCompileShader(vertexshader);
  assert(isCompileSuccess(vertexshader));

  GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentshader, 1, &fragment_shader_source, NULL);
  glCompileShader(fragmentshader);
  assert(isCompileSuccess(fragmentshader));

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexshader);
  glAttachShader(program, fragmentshader);
  glLinkProgram(program);

  assert(isLinkSuccess(program));
  glDeleteShader(vertexshader);
  glDeleteShader(fragmentshader);

  return program;
}

GLuint CreateVAO() {
  GLfloat vertices[] = {
    0.5, 0.5, 0.0,
    0.5, -0.5, 0.0,
    -0.5, -0.5, 0.0,
    -0.5, 0.5, 0.0
  };

  GLfloat indexes[] = {
    0, 1, 2,
    2, 3, 1
  };

  GLuint vbo, vao, ebo;
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);
  glGenVertexArrays(1, &vao);

  // vao setup and binding
  glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  3* sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
  glBindVertexArray(0);

  return vao;
}

int main(int argc, char* argv[]) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  //creating window
  GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpengl", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create glfw window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // making context current with window
  glfwMakeContextCurrent(window);

  //initialize glew
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to init glew " << std::endl;
    return -1;
  }

  int height, width;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  glfwSetKeyCallback(window, key_callback);

  GLuint vao = CreateVAO();

  GLint shaderprogram = CreateAndAttachShaderProgram();

  // gameloop
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    // opengl rendering
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderprogram);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}


