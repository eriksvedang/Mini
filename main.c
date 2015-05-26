#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

float tri[] = { -0.8,  0.6,    1, 0, 0,
		 0.7,  0.0,    1, 1, 1,
		-0.8, -0.6,    1, 1, 0, };

const char *vertex_shader_src = ""
"#version 330 core\n"
"layout (location = 0) in vec2 pos;"
"layout (location = 1) in vec3 col;"
"out vec3 passed_color;"
"void main() {"
"  passed_color = col;"
"  gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);"
"}";

const char *fragment_shader_src = ""
"#version 330 core\n"
"in vec3 passed_color;"
"out vec4 color;"
"void main() {"
"  color = vec4(passed_color.rgb, 1.0);"
"}";

void check_shader(GLuint shader, const char *shader_name) {
  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success) {
    GLchar info_log[512];
    glGetShaderInfoLog(shader, 512, NULL, info_log);
    printf("Error in %s: %s\n", shader_name, info_log);
  }
}

int main() {
  glfwInit();

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
  GLFWwindow *window = glfwCreateWindow(400, 300, "Mini", NULL, NULL);
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize GLEW\n");
    return -1;
  }
  
  glClearColor(0.95, 0.9, 0.95, 1.0);
  
  GLuint vs;
  vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertex_shader_src, NULL);
  glCompileShader(vs);
  check_shader(vs, "vs");
  
  GLuint fs;
  fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragment_shader_src, NULL);
  glCompileShader(fs);
  check_shader(fs, "fs");

  GLuint program;
  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if(!success) {
    GLchar info_log[512];
    glGetProgramInfoLog(program, 512, NULL, info_log);
    printf("Linker error: %s\n", info_log);
  }

  glUseProgram(program);
  
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  
  GLuint vbo;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(tri), tri, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
  
  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteShader(vs);
  glDeleteShader(fs);
  glfwTerminate();
}

