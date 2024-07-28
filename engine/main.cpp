#include "process_input.hpp"
#include "shaders.hpp"

#define GLAD_GL_IMPLEMENTATION
// Glad header from GLFW.
#include <glad/gl.h>

#undef GLAD_GL_IMPLEMENTATION

#include <glfw/glfw3.h>

#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <plog/Init.h>
#include <plog/Log.h>

#include <cstdint>

void framebuffer_size_changed_cb(GLFWwindow *window, std::int32_t width,
                                 std::int32_t height) {
  PLOG_VERBOSE << "new width " << width << " new height " << height;
  glViewport(0, 0, width, height);
}

void logGlfwError() {
  const char *error = nullptr;
  if (glfwGetError(&error))
    PLOG_ERROR << "GLFW Error: " << error;
}

int main() {
  static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::verbose, &consoleAppender);

  PLOG_VERBOSE << "start glfw init";
  if (!glfwInit()) {
    PLOG_ERROR << "failed to init glfw";
    logGlfwError();
    return -1;
  }
  PLOG_VERBOSE << "finish glfw init";

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(__APPLE__)
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  auto *window = glfwCreateWindow(800, 600, "fornix", nullptr, nullptr);
  if (window == nullptr) {
    PLOG_ERROR << "failed to create a window";
    logGlfwError();
    glfwTerminate();
    return -2;
  }
  // Tell GLFW to make the context of the window main on the current thread.
  glfwMakeContextCurrent(window);

  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    PLOG_ERROR << "GLAD failed to load OpenGL functions";
    return -3;
  }
  // Tell OpenGL the size of the rendering window.
  glViewport(0, 0, 800, 600);
  // Screen size changed.
  glfwSetFramebufferSizeCallback(window, framebuffer_size_changed_cb);

  // (1) Allocate Vertex Array Object.
  GLuint VAO = 0;
  // 1. Ask OpenGL to allocate one Vertex Array Object.
  glGenVertexArrays(1, &VAO);
  // 1.1. Make the VAO active/current.
  glBindVertexArray(VAO);

  // (2) Allocate Vertex Buffer Object
  const float triangle[]{
      0.0f,  0.5f,  0.0f, // Mid top.
      -0.5f, -0.5f, 0.0f, // Left bottom.
      0.5f,  -0.5f, 0.0f, // Right bottom.
  };
  GLuint VBO = 0;
  // 2. Ask OpenGL to allocate 1 Buffer Object (that's just a generic buffer).
  glGenBuffers(1, &VBO);
  // 2.1 Bind the buffer to the GL_ARRAY_BUFFER context (which also tells that
  // we're going to store vertex attributes in the buffer) and binds it to the
  // current VAO.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 2.3 Copy the data in the buffer currently bound to GL_ARRAY_BUFFER target.
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

  // 3.1 Specify how currently bound Vertex Buffer Object (GL_ARRAY_BUFFER)
  // should be splitted into attributes for the Vertex Shader.
  // Note: this information is stored in the current VAO.
  constexpr auto ATTRIBUTE_INDEX = 0;
  glVertexAttribPointer(
      ATTRIBUTE_INDEX, // Index of the input attribute (as specified in the
                       // vertex shader).
      3,               // Number of component per attribute.
      GL_FLOAT,        // Type of the each component in the attribute.
      GL_FALSE, // Should the data be clamped/normalized to the range [-1, 1]
                // for signed and [0, 1] for unsigned.
      sizeof(float) * 3, // The size of one vertex attrbute.
      nullptr            // Offset of the first attribute.
  );
  // 3.2 Enable the vertex attribute.
  glEnableVertexAttribArray(ATTRIBUTE_INDEX // Index of the input attribute (as
                                            // specified in the vertex shader).
  );

  // 4.1 Compile & link the shader program.
  std::uint32_t shader_program_id = 0;
  if (!fornix::shaders::opengl::compile_and_link_all(shader_program_id)) {
    PLOG_FATAL << "failed to compile/link the shader program";
    return -4;
  }
  // 4.2 Use the compiled & linked shader program.
  glUseProgram(shader_program_id);

  // Render loop.

  while (!glfwWindowShouldClose(window)) {
    fornix::process_input(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, // Index of VAO.
                 3                // Number of indicies to be rendered.
    );
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
