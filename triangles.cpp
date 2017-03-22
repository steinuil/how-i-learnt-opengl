#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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
    window = glfwCreateWindow(800, 600, "sup Mugi", nullptr, nullptr);

    if (window == nullptr) {
      std::cout << "rip glfw window" << std::endl;
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

    { const GLchar* vertexShaderSource = R"(
      #version 330

      layout (location = 0) in vec3 position;

      void main() {
        gl_Position = vec4(position.x, position.y, position.z, 1.0);
      }
      )";

      glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
      glCompileShader(vertexShader);

      glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "rip vertex shader" << std::endl << infoLog << std::endl;
      }
    }

    { GLuint fragmentShaders[2];
      const GLchar* fragShaderSources[2] = {
        R"(#version 330
        out vec4 color;

        void main() {
          color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
        }
        )",
        R"(#version 330
        out vec4 color;

        void main() {
          color = vec4(0.2f, 1.0f, 0.2f, 1.0f);
        }
        )"
      };

      for (int i = 0; i < 2; i++) {
        fragmentShaders[i] = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(fragmentShaders[i], 1, &fragShaderSources[i], nullptr);
        glCompileShader(fragmentShaders[i]);

        glGetShaderiv(fragmentShaders[i], GL_COMPILE_STATUS, &success);
        if (!success) {
          glGetShaderInfoLog(fragmentShaders[i], 512, nullptr, infoLog);
          std::cout << "rip fragment shader" << std::endl << infoLog << std::endl;
        }

        // Link shaders
        shaders[i] = glCreateProgram();

        glAttachShader(shaders[i], vertexShader);
        glAttachShader(shaders[i], fragmentShaders[i]);
        glLinkProgram(shaders[i]);

        glGetProgramiv(shaders[i], GL_LINK_STATUS, &success);
        if (!success) {
          glGetShaderInfoLog(fragmentShaders[i], 512, nullptr, infoLog);
          std::cout << "rip shader program" << std::endl << infoLog << std::endl;
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

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
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

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
      glEnableVertexAttribArray(0);
      glBindVertexArray(0);
    }
  }



  glfwSetKeyCallback(window, key_callback);

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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
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
