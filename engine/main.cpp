#define GLAD_GL_IMPLEMENTATION
// Glad header from GLFW.
#include <glad/gl.h>

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
  glfwSetFramebufferSizeCallback(window, framebuffer_size_changed_cb);

  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}