#include "process_input.hpp"

#include <glfw/glfw3.h>

namespace fornix {
void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
} // namespace fornix