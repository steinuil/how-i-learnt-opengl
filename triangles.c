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

GLuint *shaders;

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



  // Testing matrix stuff
  { vec3_t a = vec3_transform(
      mat4_translate(vec3(1.0, 1.0, 0.0)),
      vec3(1.0, 0.0, 0.0)
    );
    printf("%f %f %f\n", a.x, a.y, a.z);

    mat4_t trans = mat4_mul(
      mat4_rotate_z(deg_to_rad(90.0)),
      mat4_scale(vec3(0.5, 0.5, 0.5))
    );
  }



  // Load shaders
  { char *files[] = {
      "shaders/vertex.vs",
      "shaders/texture-vertex.vs",
      "shaders/orange.fs",
      "shaders/green.fs",
      "shaders/texture-frag.fs",
    };

    int indexes[] = {
      0, 2,
      0, 3,
      1, 4,
    };

    shaders = loadShaders(files, arrayLength(files), indexes, arrayLength(indexes) / 2);
  }



  // Vertex stuff, I don't know man
  GLuint VAOs[3];

  { GLuint VBOs[3], EBOs[2];
    glGenVertexArrays(3, VAOs);
    glGenBuffers(3, VBOs);
    glGenBuffers(2, EBOs);

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

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)NULL);
      glEnableVertexAttribArray(0);
      glBindVertexArray(0);
    }

    { GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,   0.0f, 0.5f, 0.2f,
        0.5f, -0.5f, 0.0f,  0.0f, 0.5f, 0.2f,
        0.7f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f,
      };

      glBindVertexArray(VAOs[1]);
      glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
      glEnableVertexAttribArray(0);

      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
      glEnableVertexAttribArray(1);

      glBindVertexArray(0);
    }

    { GLfloat vertices[] = {
        0.4f, 0.4f, 0.0f,   2.0f, 0.0f,
        0.4f, -0.4f, 0.0f,  2.0f, 2.0f,
        -0.4f, -0.4f, 0.0f, 0.0f, 2.0f,
        -0.4f, 0.4f, 0.0f,  0.0f, 0.0f,
      };

      GLuint indices[] = {
        0, 1, 3,
        1, 2, 3,
      };

      glBindVertexArray(VAOs[2]);
      glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
      glEnableVertexAttribArray(0);

      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
      glEnableVertexAttribArray(1);

      glBindVertexArray(0);
    }
  }



  // Load textures
  GLuint *textures;

  { struct textureOpts options[] = {
      { "textures/todd.png",       GL_CLAMP_TO_EDGE,   GL_CLAMP_TO_EDGE },
      { "textures/watermelon.png", GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, },
    };

    textures = load2DTextures(options, arrayLength(options));
  }



  glfwSetKeyCallback(window, keyCallback);

  glUseProgram(shaders[2]);
  glUniform1f(glGetUniformLocation(shaders[2], "blending"), 0.2f);

  while (!glfwWindowShouldClose(window)) {
    // Won't receive the key callback thing if this doesn't run
    glfwPollEvents();

    GLfloat time = glfwGetTime();

    // Set background color
    glClearColor(sin(time * 2), 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw first thing
    glBindVertexArray(VAOs[0]);
    glUseProgram(shaders[0]);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

    // Draw second triangle
    glBindVertexArray(VAOs[1]);
    glUseProgram(shaders[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Draw texture triangle
    glUseProgram(shaders[2]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glUniform1i(glGetUniformLocation(shaders[2], "theTexture"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glUniform1i(glGetUniformLocation(shaders[2], "theTexture"), 1);

    glBindVertexArray(VAOs[2]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
  if (action != GLFW_PRESS) return;
  switch (key) {
    case GLFW_KEY_Q: {
      glfwSetWindowShouldClose(window, GL_TRUE);
      break;
    }

    case GLFW_KEY_SPACE: {
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

    case GLFW_KEY_UP: {
      GLint loc = glGetUniformLocation(shaders[2], "blending");
      GLfloat blending;
      glGetUniformfv(shaders[2], loc, &blending);

      if (blending < 1.0f) blending += 0.1f;

      glUniform1f(loc, blending);
      break;
    }

    case GLFW_KEY_DOWN: {
      GLint loc = glGetUniformLocation(shaders[2], "blending");
      GLfloat blending;
      glGetUniformfv(shaders[2], loc, &blending);

      if (blending > 0.1f) blending -= 0.1f;

      glUniform1f(loc, blending);

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
