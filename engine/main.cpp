#include "process_input.hpp"
#include "shader_program_opengl.hpp"

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
#include <utility>

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#endif

std::filesystem::path get_exe_path() {
  char path[PATH_MAX];

#ifdef _WIN32
  // Windows implementation
  GetModuleFileName(NULL, path, MAX_PATH);
#elif __linux__
  // Linux implementation
  ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
  if (count == -1) {
    throw std::runtime_error("Failed to get executable path");
  }
  path[count] = '\0';
#elif __APPLE__
  // macOS implementation
  uint32_t size = sizeof(path);
  if (_NSGetExecutablePath(path, &size) != 0) {
    throw std::runtime_error("Buffer too small; size needed: " +
                             std::to_string(size));
  }
#endif

  return std::filesystem::path(path).parent_path();
}

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

void enable_vertex_attribute(const std::int32_t attribute_index,
                             const std::int32_t stride_size,
                             const void *offset) {
  glVertexAttribPointer(
      attribute_index, // Index of the input attribute (as specified in the
                       // vertex shader).
      3,               // Number of component per attribute.
      GL_FLOAT,        // Type of the each component in the attribute.
      GL_FALSE,    // Should the data be clamped/normalized to the range [-1, 1]
                   // for signed and [0, 1] for unsigned.
      stride_size, // The size of one vertex attrbute.
      offset       // Offset of the first attribute.
  );
  // 3.2 Enable the vertex attribute.
  glEnableVertexAttribArray(attribute_index // Index of the input attribute (as
                                            // specified in the vertex shader).
  );
}

void setup_vao(const std::vector<float> &verticies,
               std::uint32_t &out_vao_index) {
  // (1) Allocate Vertex Array Object.
  // 1. Ask OpenGL to allocate one Vertex Array Object.
  glGenVertexArrays(1, &out_vao_index);
  PLOG_INFO << "vao index " << out_vao_index;
  // 1.1. Make the VAO active/current.
  glBindVertexArray(out_vao_index);

  // 2. Ask OpenGL to create 1 Buffer Object.
  GLuint VBO = 0;
  // 2. Ask OpenGL to allocate 1 Buffer Object (that's just a generic buffer).
  glGenBuffers(1, &VBO);
  // 2.1 Bind the buffer to the GL_ARRAY_BUFFER context (which also tells that
  // we're going to store vertex attributes in the buffer) and binds it to the
  // current VAO.
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 2.3 Copy the data in the buffer currently bound to GL_ARRAY_BUFFER target.
  glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float),
               verticies.data(), GL_STATIC_DRAW);

  // 3.1 Specify how currently bound Vertex Buffer Object (GL_ARRAY_BUFFER)
  // should be splitted into attributes for the Vertex Shader.
  // Note: this information is stored in the current VAO.
  enable_vertex_attribute(0, sizeof(float) * 6, nullptr);
  enable_vertex_attribute(1, sizeof(float) * 6, (void *)(sizeof(float) * 3));
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

  // (2) Allocate Vertex Buffer Object
  const float triangle[]{
      -1.0, 0.0, 0.0, // Left.
      -0.5, 0.5, 0.0, // Top.
      0.0, 0.0, 0.0,  // Right.
                      // Second triangle.
      0.0, 0.0, 0.0,  // Left.
      0.5, 0.5, 0.0,  // Top.
      1.0, 0.0, 0.0   // Right.

  };
  GLuint vao1, vao2;
  setup_vao(
      {
          // Color.
          -1.0, 0.0, 0.0, 1.0, 1.0, 1.0, // Left.
          -0.5, 0.5, 0.0, 1.0, 1.0, 0.0, // Top.
          0.0, 0.0, 0.0, 1.0, 1.0, 1.0,  // Right.
      },
      vao1);
  setup_vao(
      {
          // Color
          0.0, 0.0, 0.0, 1.0, 0.0, 0.0, // Left.
          0.5, 0.5, 0.0, 0.0, 1.0, 0.0, // Top.
          1.0, 0.0, 0.0, 0.0, 0.0, 1.0, // Right.
      },
      vao2);
  // const std::uint32_t indices[] = {
  //     // note that we start from 0!
  //     0, 1, 3, // first triangle
  //     1, 2, 3  // second triangle
  // };
  // GLuint EBO = 0;
  // // Ask OpenGL to create 1 Buffer Object.
  // glGenBuffers(1, &EBO);
  // // Bind to the current VAO.
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // // Copy the indicies into the VBO.
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
  //              GL_STATIC_DRAW);

  // 4.1 Compile & link the shader program.
  // std::uint32_t shader_program_id_1 = 0, shader_program_id_2 = 0;
  // if (!fornix::shaders::opengl::compile_and_link_all(shader_program_id_1)) {
  //   PLOG_FATAL << "failed to compile/link the shader program 1";
  //   return -4;
  // }
  // if (!fornix::shaders::opengl::compile_and_link_all(shader_program_id_2,
  //                                                    nullptr,
  //                                                    R"(
  //   #version 330 core
  //   out vec4 FragColor;
  //   uniform vec4 app_color;
  //   in vec4 vertex_color;

  //   void main()
  //   {
  //       //FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
  //       //FragColor = app_color;
  //       FragColor = vertex_color;
  //   }
  //   )")) {
  //   PLOG_FATAL << "failed to compile/link the shader program 2";
  //   return -4;
  // }
  // 4.2 Use the compiled & linked shader program.
  // glUseProgram(shader_program_id);
  const auto exe = get_exe_path();
  fornix::shader_program_opengl program{exe / "resources" / "shader.vs",
                                        exe / "resources" / "shader.fs"};
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //  Render loop.
  while (!glfwWindowShouldClose(window)) {
    fornix::process_input(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    for (const auto &[vao, shader] : {std::pair(vao1, program)}) {
      shader.activate();

      // const auto time = glfwGetTime();
      // auto greenVal = (sin(time) / 2.0) + 0.5;
      // const auto color_loc = glGetUniformLocation(shader, "app_color");
      // glUniform4f(color_loc, 0.0, greenVal, 0.0, 1.0);
      glBindVertexArray(vao);
      glDrawArrays(GL_TRIANGLES, 0, // Index of VAO.
                   4                // Number of indicies to be rendered.
      );
    }

    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
