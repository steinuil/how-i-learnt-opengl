#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL.h>
#include <vec.h>

#define arrayLength(x) (sizeof(x) / sizeof(x[0]))

struct textureOpts {
  char *file;
  GLint wrapS;
  GLint wrapT;
};

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
GLchar* readFile(char *name);
GLuint* loadShaders(char **files, int fileCount, int *indexes, int shaderCount);
GLuint* load2DTextures(struct textureOpts *options, int textureCount);

vec3_t position = { 0.0, 0.0, -3.0 };

float pitch = 0.0,
      yaw = 0.0;

//struct { float offsetX, offsetY; double lastX, lastY } camera = { 0.0 };


int main(void) {
  // Create window and bind the current context to it
  GLFWwindow* window;

  { glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
  int width, height;

  { glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
      return -1;
    }

    // In high DPI screns the framebuffer will be bigger than the window size.
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
  }



  // Load shaders
  GLuint *shaders;

  { char *files[] = {
      "shaders/texture-vertex.vs",
      "shaders/texture-frag2.fs",
    };

    int indexes[] = {
      0, 1,
    };

    shaders = loadShaders(files, arrayLength(files), indexes, arrayLength(indexes) / 2);
  }



  // Load textures
  GLuint *textures;

  { struct textureOpts options[] = {
      { "textures/todd.png",       GL_CLAMP_TO_EDGE,   GL_CLAMP_TO_EDGE },
      //{ "textures/watermelon.png", GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, },
    };

    textures = load2DTextures(options, arrayLength(options));
  }



  GLuint VAO;

  { GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    GLfloat vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
  }



  glfwSetKeyCallback(window, keyCallback);

  float mouseOffset_x = 0.0,
        mouseOffset_y = 0.0;

  double lastx, lasty;
  glfwGetCursorPos(window, &lastx, &lasty);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    float time = glfwGetTime();

    { double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);

      mouseOffset_x = lastx - xpos;
      mouseOffset_y = lasty - ypos;

      lastx = xpos;
      lasty = ypos;

      //printf("%f %f\n", mouseOffset_x, mouseOffset_y);
    }

    // Set background color
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaders[0]);

    // Set texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glUniform1i(glGetUniformLocation(shaders[0], "toddTexture"), 0);

    // Camera position
    { const float sensitivity = 2.0;
      yaw   -= (mouseOffset_x / sensitivity);
      pitch -= (mouseOffset_y / sensitivity);

      mat4_t view = mat4_mul(
        mat4_rotate_x(deg_to_rad(pitch)),
        mat4_mul(
          mat4_rotate_y(deg_to_rad(yaw)),
          mat4_translate(position)
        )
      );

      //printf("%f\n", yaw);

      glUniformMatrix4fv(
        glGetUniformLocation(shaders[0], "view"),
        1, GL_FALSE, (const GLfloat *)view.ary
      );
    }

    // Model position
    { mat4_t model = mat4_mul(
        mat4_translate(vec3(0.0, -.5, -1.0)),
        mat4_rotate_y(deg_to_rad(time * 45.0))
      );

      glUniformMatrix4fv(
        glGetUniformLocation(shaders[0], "model"),
        1, GL_FALSE, (const GLfloat *)model.ary
      );
    }

    // Perspective
    { mat4_t projection = mat4_perspective(
        deg_to_rad(45.0), (float)width / (float)height, 0.1, 100.0
      );

      glUniformMatrix4fv(
        glGetUniformLocation(shaders[0], "projection"),
        1, GL_FALSE, (const GLfloat *)projection.ary
      );
    }

    // Draw the array
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
  }

  glfwTerminate();
  return 0;
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
  //if (action != GLFW_PRESS) return;
  switch (key) {
    case GLFW_KEY_Q: {
      glfwSetWindowShouldClose(window, GL_TRUE);
      break;
    }

    case GLFW_KEY_BACKSPACE: {
      GLint mode;

      glGetIntegerv(GL_POLYGON_MODE, &mode);

      switch (mode) {
      case GL_LINE: mode = GL_FILL;  break;
      case GL_FILL: mode = GL_POINT; break;
      default:      mode = GL_LINE;  break;
      }

      glPolygonMode(GL_FRONT_AND_BACK, mode);
      break;
    }

    case GLFW_KEY_A: {
      position.x += 0.1 * cos(deg_to_rad(yaw));
      position.z += 0.1 * sin(deg_to_rad(yaw));
      break;
    }

    case GLFW_KEY_D: {
      position.x -= 0.1 * cos(deg_to_rad(yaw));
      position.z -= 0.1 * sin(deg_to_rad(yaw));
      break;
    }

    case GLFW_KEY_W: {
      position.z += 0.1 * cos(deg_to_rad(yaw));
      position.x -= 0.1 * sin(deg_to_rad(yaw));
      break;
    }

    case GLFW_KEY_S: {
      position.z -= 0.1 * cos(deg_to_rad(yaw));
      position.x += 0.1 * sin(deg_to_rad(yaw));
      break;
    }

    case GLFW_KEY_LEFT_CONTROL: {
      position.y += 0.1;
      break;
    }

    case GLFW_KEY_SPACE: {
      position.y -= 0.1;
      break;
    }
  }
}



GLchar* readFile(char *name) {
  long length;
  GLchar *buffer;

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



GLuint* loadShaders(char **files, int fileCount, int *indexes, int shaderCount) {
  GLuint *shaders = (GLuint *) calloc(sizeof(GLuint), shaderCount);
  GLchar *shaderFiles[fileCount];
  GLuint compiledShaders[fileCount];

  // Might read more files than it needs if the files array includes stuff we don't need
  // but I guess we're gonna have to live with that.
  for (int i = 0; i < fileCount; i++) {
    shaderFiles[i] = readFile(files[i]);
  }

  for (int i = 0; i < shaderCount; i++) {
    GLint success; GLchar infoLog[512];

    shaders[i] = glCreateProgram();

    // Compile both shaders
    for (int s = 0; s < 2; s++) {
      int current = indexes[(i * 2) + s];

      // Compile the shader only if needed
      if (shaderFiles[current]) {
        compiledShaders[current] = glCreateShader(s == 0 ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

        glShaderSource(compiledShaders[current], 1, (const GLchar **)&shaderFiles[current], NULL);
        glCompileShader(compiledShaders[current]);

        glGetShaderiv(compiledShaders[current], GL_COMPILE_STATUS, &success);
        if (!success) {
          glGetShaderInfoLog(compiledShaders[current], 512, NULL, infoLog);
          printf("rip vertex shader\n%s", infoLog);
        }

        free(shaderFiles[current]);
        shaderFiles[current] = NULL;
      }

      glAttachShader(shaders[i], compiledShaders[current]);
    }

    // Link
    glLinkProgram(shaders[i]);

    glGetProgramiv(shaders[i], GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shaders[i], 512, NULL, infoLog);
      printf("rip shader program %d\n%s", 2, infoLog);
    }
  }

  // Clean up
  for (int i = 0; i < fileCount; i++) {
    if (shaderFiles[i]) free(shaderFiles[i]);
    if (compiledShaders[i]) glDeleteShader(compiledShaders[i]);
  }

  return shaders;
}



GLuint* load2DTextures(struct textureOpts *options, int textureCount) {
  GLuint *textures = (GLuint *) calloc(sizeof(GLuint), textureCount);
  glGenTextures(textureCount, textures);

  for (int i = 0; i < textureCount; i++) {
    glBindTexture(GL_TEXTURE_2D, textures[i]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, options[i].wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, options[i].wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char *image = SOIL_load_image(options[i].file, &width, &height, NULL, SOIL_LOAD_RGBA);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    SOIL_free_image_data(image);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  return textures;
}
