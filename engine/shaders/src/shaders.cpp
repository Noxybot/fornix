#include "shaders.hpp"

#include <glad/gl.h>

#include <plog/Log.h>

#include <cstdint>

namespace fornix::shaders::opengl {
static const char *VERTEX_SHADER_SOURCE = //
    R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    } 
    )";
static const char *FRAGMENT_SHADER_SOURCE = //
    R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
    )";

static bool check_compile_result(GLuint shader_id) {
  GLint success = 0;
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  if (!success) {
    char info[512];
    glGetShaderInfoLog(shader_id, 512, nullptr, info);
    PLOG_ERROR << "failed to compile the shader with id " << shader_id
               << ", ERROR:\n"
               << info;
    return false;
  }
  return true;
}
static bool check_link_result(GLuint program_id) {
  GLint success = 0;
  glGetProgramiv(program_id, GL_LINK_STATUS, &success);
  if (!success) {
    char info[512];
    glGetShaderInfoLog(program_id, 512, nullptr, info);
    PLOG_ERROR << "failed to compile the program with id " << program_id
               << ", ERROR:\n"
               << info;
    return false;
  }
  PLOG_INFO << "OpenGL shader program linked successfully";
  return true;
}

static bool compile(const char *source, GLuint shader_type,
                    GLuint &out_shader_id) {
  // Create the shader object.
  out_shader_id = glCreateShader(shader_type);
  // Assign source code to the shader object.
  glShaderSource(out_shader_id, 1, &source,
                 nullptr // If length is NULL, each string is assumed to be null
                         // terminated.
  );
  glCompileShader(out_shader_id);
  return check_compile_result(out_shader_id);
}

// static bool link(GLuint shader_program_id, const std::vector<GLuint>&
// shader_ids) {
//     for (const auto& shader_id : shader_ids) {

//     }
// }

bool compile_and_link_all(std::uint32_t &out_shader_program_id) {

  GLuint vertex_shader_id = 0, fragment_shader_id = 0;
  if (!compile(VERTEX_SHADER_SOURCE, GL_VERTEX_SHADER, vertex_shader_id) ||
      !compile(FRAGMENT_SHADER_SOURCE, GL_FRAGMENT_SHADER,
               fragment_shader_id)) {
    return true;
  }
  PLOG_INFO << "OpenGL shaders compiled successfully";
  // Create a shader program and attach compiled shaders to it.
  out_shader_program_id = glCreateProgram();
  glAttachShader(out_shader_program_id, vertex_shader_id);
  glAttachShader(out_shader_program_id, fragment_shader_id);
  glLinkProgram(out_shader_program_id);

  // Delete the shader objects - we no longer need them after linking.
  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  return check_link_result(out_shader_program_id);
}

} // namespace fornix::shaders::opengl