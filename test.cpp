//#define GLEW_STATIC
#include <iostream>
#include <GL/glew.h>

#include <GLFW/glfw3.h>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(void) {
  glfwInit();
  // Set GL version 3.3.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // The core profile complains if you use deprecated features.
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


  // Can make the window fullscreen by passing a monitor as the 4th argument
  // to the glfwCreateWindow function.
  /*
  int monitorCount;
  GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
  */

  GLFWwindow* window = glfwCreateWindow(800, 600, "sup Mugi", nullptr, nullptr);

  if (window == nullptr) {
    std::cout << "rip glfw window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // glewExperimental should be set to true
  // before starting glew.
  // Otherwise, problems with the core profile might happen.
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    return -1;
  }

  // In high DPI screns the framebuffer will be bigger than the window size.
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  glViewport(0, 0, width, height);



  // AH

  const GLchar* vertexShaderSource = "#version 330\n"
    "layout (location = 0) in vec3 position;\n"
    //"in vec3 position;\n"
    "void main() {\n"
    "  gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}\n\0";

  const GLchar* fragmentShaderSource = "#version 330\n"
    "out vec4 color;\n"
    "void main() {\n"
    "  color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // Check for compile time errors
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  // Fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // Check for compile time errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
  // Link shaders
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // Check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);


  // Set up vertex data (and buffer(s)) and attribute pointers
  GLfloat vertices[] = {
    -0.5f, -0.5f, 0.0f, // Left  
    0.5f, -0.5f, 0.0f, // Right 
    0.0f,  0.5f, 0.0f  // Top   
  };
  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), static_cast<GLvoid*>(nullptr));
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

  glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


  // OH

  glfwSetKeyCallback(window, key_callback);

  while (!glfwWindowShouldClose(window)) {
    // Won't receive the key callback thing if this doesn't run
    glfwPollEvents();

    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
  }
}
