#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLchar* readFile(char *name);

int main(void) {
  // Create window and bind the current context to it
  GLFWwindow* window;

  { glfwInit();

    // Set GL version 3.3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // The core profile complains if you use deprecated features.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Can make the window fullscreen by passing a monitor as the 4th argument
    window = glfwCreateWindow(800, 600, "Triangles", NULL, NULL);

    if (window == NULL) {
      printf("rip glfw window\n");
      glfwTerminate();
      return -1;
    }

    glfwMakeContextCurrent(window);
  }



  // Initiate OpenGL proper
  { glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
      return -1;
    }

    // In high DPI screns the framebuffer will be bigger than the window size.
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
  }



  // Shaders
  GLuint shaders[2];

  { GLint success;
    GLchar infoLog[512];
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    { const GLchar *vertexShaderSource = readFile("shader.vs");
      if (!vertexShaderSource) {
        puts("rip vertex shader file");
      }

      glShaderSource(vertexShader, 1, (const GLchar **)&vertexShaderSource, NULL);
      glCompileShader(vertexShader);

      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("rip vertex shader\n%s", infoLog);
      }
    }

    { GLuint fragmentShaders[2];
      const GLchar* fragShaderSources[2] = {
        readFile("orange.fs"),
        readFile("green.fs"),
      };

      for (int i = 0; i < 2; i++) {
        if (!fragShaderSources[i]) {
          printf("could not open fragment shader %d\n", i);
        }

        fragmentShaders[i] = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragmentShaders[i], 1, &fragShaderSources[i], NULL);
        glCompileShader(fragmentShaders[i]);

        glGetShaderiv(fragmentShaders[i], GL_COMPILE_STATUS, &success);
        if (!success) {
          glGetShaderInfoLog(fragmentShaders[i], 512, NULL, infoLog);
          printf("rip fragment shader %d\n%s", i, infoLog);
        }

        // Link shaders
        shaders[i] = glCreateProgram();

        glAttachShader(shaders[i], vertexShader);
        glAttachShader(shaders[i], fragmentShaders[i]);
        glLinkProgram(shaders[i]);

        glGetProgramiv(shaders[i], GL_LINK_STATUS, &success);
        if (!success) {
          glGetShaderInfoLog(fragmentShaders[i], 512, NULL, infoLog);
          printf("rip shader program %d\n%s", i, infoLog);
        }

        glDeleteShader(fragmentShaders[i]);
      }
    }

    glDeleteShader(vertexShader);
  }



  // Vertex stuff, I don't know man
  GLuint VAOs[2];

  { GLuint VBOs[2], EBO;
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    glGenBuffers(1, &EBO);

    { GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f,  // Top Right
         0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  // Bottom Left
        -0.5f,  0.5f, 0.0f,   // Top Left
        -0.7f,  0.0f, 0.0f,
      };

      GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
        2, 3, 4,
      };

      glBindVertexArray(VAOs[0]);
      glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)NULL);
      glEnableVertexAttribArray(0);
      glBindVertexArray(0);
    }

    { GLfloat vertices2[] = {
        0.5f, 0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.7f, 0.0f, 0.0f,
      };

      glBindVertexArray(VAOs[1]);
      glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)NULL);
      glEnableVertexAttribArray(0);
      glBindVertexArray(0);
    }
  }



  glfwSetKeyCallback(window, keyCallback);

  while (!glfwWindowShouldClose(window)) {
    // Won't receive the key callback thing if this doesn't run
    glfwPollEvents();

    // Set background color
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw first thing
    glBindVertexArray(VAOs[0]);
    glUseProgram(shaders[0]);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    // Draw second triangle
    glBindVertexArray(VAOs[1]);
    glUseProgram(shaders[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Unbind the vertex array
    glBindVertexArray(0);

    // Not running this makes the window quite slow for some reason.
    // Also, nothing is rendered.
    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  } else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
    GLint mode;

    glGetIntegerv(GL_POLYGON_MODE, &mode);

    switch(mode) {
    case GL_LINE: mode = GL_FILL;  break;
    case GL_FILL: mode = GL_POINT; break;
    default:      mode = GL_LINE;  break;
    }

    glPolygonMode(GL_FRONT_AND_BACK, mode);
  }
}

GLchar* readFile(char *name) {
  long length;
  GLchar *buffer;

  // Open file
  FILE *file = fopen(name, "r");
  if (!file) return NULL;

  fseek(file, 0, SEEK_END);
  length = ftell(file);
  rewind(file);

  buffer = (GLchar *) calloc(sizeof(GLchar), length + 1);

  fread(buffer, sizeof(GLchar), length, file);
  buffer[length] = '\0';
  fclose(file);

  return buffer;
}
