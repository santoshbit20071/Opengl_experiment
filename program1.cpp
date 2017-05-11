#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL.h>

class Program {
  public:
    Program(const std::string& vertexPath, const std::string& fragmentPath) {

       std::ifstream vShaderFile;
       std::ifstream fShaderFile;
       std::string vertexCode,fragmentCode;
       vShaderFile.exceptions (std::ifstream::badbit);
       fShaderFile.exceptions (std::ifstream::badbit);
       try
       {
         vShaderFile.open(vertexPath);
         fShaderFile.open(fragmentPath);
         std::stringstream vShaderStream, fShaderStream;
         // Read file's buffer contents into streams
         vShaderStream << vShaderFile.rdbuf();
         fShaderStream << fShaderFile.rdbuf();
         vShaderFile.close();
         fShaderFile.close();
         // Convert stream into string
         vertexCode = vShaderStream.str();
         fragmentCode = fShaderStream.str();
         }
         catch (std::ifstream::failure e)
         {
         std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
         }

      const GLchar* vertex_source = vertexCode.c_str();
      const GLchar* fragment_source = fragmentCode.c_str();
      std::cout << "HELLO : " << fragmentCode << std::endl;


      GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertexshader, 1, &vertex_source, NULL);
      glCompileShader(vertexshader);
      assert(isCompileSuccess(vertexshader));

      GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragmentshader, 1, &fragment_source, NULL);
      glCompileShader(fragmentshader);
      assert(isCompileSuccess(fragmentshader));

      shader_program_ = glCreateProgram();
      glAttachShader(shader_program_, vertexshader);
      glAttachShader(shader_program_, fragmentshader);
      glLinkProgram(shader_program_);

      assert(isLinkSuccess(shader_program_));
      glDeleteShader(vertexshader);
      glDeleteShader(fragmentshader);
    }

    void Use() {
      glUseProgram(shader_program_);
    }

  private:
    GLuint shader_program_;

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
};

GLuint CreateVAO() {
  GLfloat vertices[] = {
      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
      -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
  };

 GLuint indices[] = {  // Note that we start from 0!
     0, 1, 3, // First Triangle
     1, 2, 3  // Second Triangle
 };


  GLuint vbo1, vao1, ebo1;
  glGenBuffers(1, &vbo1);
  glGenBuffers(1, &ebo1);
  glGenVertexArrays(1, &vao1);

  // vao setup and binding
  glBindVertexArray(vao1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,  8* sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  8* sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  8* sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  return vao1;
}

class GLFWRunner {
  public:
    void Init() {
      glfwInit();
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    }

    void Terminate() {
      glfwTerminate();
    }

    GLFWwindow* CreateWindow() {
      GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpengl", nullptr, nullptr);
      if (window == nullptr) {
        std::cout << "Failed to create glfw window" << std::endl;
        return nullptr;
      }
    }

    void InitWindow(GLFWwindow* win) {
      int height, width;
      glfwGetFramebufferSize(win, &width, &height);
      glViewport(0, 0, width, height);
      glfwSetKeyCallback(win, key_callback);
    }

    void MakeCurrent(GLFWwindow* window) {
      glfwMakeContextCurrent(window);
    }

    void Run(GLFWwindow* window, Program& shader_program, GLuint vao, GLuint tid) {
      while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // opengl rendering
        shader_program.Use();
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, tid);
        //glDrawArrays(GL_TRIANGLES, 0, 4);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        // second traingle
        glBindVertexArray(0);

        glfwSwapBuffers(window);
      }
    }

  private:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
    {
        if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
           glfwSetWindowShouldClose(window, GL_TRUE);
    }

};

GLuint CreateTexture() {
  int width, height;
  unsigned char* image = SOIL_load_image("../wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);

  GLuint tid;
  glGenTextures(1, &tid);
  glBindTexture(GL_TEXTURE_2D, tid);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  SOIL_free_image_data(image);
  glBindTexture(GL_TEXTURE_2D, 0);
  return tid;
}

int main(int argc, char* argv[]) {
  GLFWRunner glfw;
  glfw.Init();
  GLFWwindow* window = glfw.CreateWindow();
  glfw.InitWindow(window);
  glfw.MakeCurrent(window);

  glewExperimental = GL_TRUE;
  GLenum err =  glewInit();
  if (err != GLEW_OK) {
    std::cout << "Failed to init glew " << std::endl;
    std::cout << glewGetErrorString(err) << std::endl;
  }

  GLuint vao = CreateVAO();
  GLuint tid = CreateTexture();
  Program shader_program("../vertexshader.vs", "../fragmentshader.frag");

  glfw.Run(window, shader_program, vao, tid);

  glfw.Terminate();
  return 0;
}


