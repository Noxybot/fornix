#include "shader_program_opengl.hpp"

#include <glad/gl.h>
#include <plog/Log.h>

#include <fstream>

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

static std::string read_file_to_string(const std::filesystem::path &path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file.is_open()) {
    PLOG_ERROR << "can't open shader file " << path.string();
    return {};
  }

  std::streamsize file_size = file.tellg();
  file.seekg(0, std::ios::beg);

  std::string body;
  body.reserve(file_size);

  body.assign((std::istreambuf_iterator<char>(file)),
              std::istreambuf_iterator<char>());

  return body;
}

namespace fornix {
shader_program_opengl::shader_program_opengl(
    const std::filesystem::path &vertex_shader,
    const std::filesystem::path &fragment_shader) {
  const auto vertex_shader_src = read_file_to_string(vertex_shader);
  const auto frag_shader_src = read_file_to_string(fragment_shader);
  std::int32_t vertex_shader_id = -1, frag_shader_id = 0;
  if (!compile_shader(vertex_shader_src, GL_VERTEX_SHADER, vertex_shader_id) ||
      !compile_shader(frag_shader_src, GL_FRAGMENT_SHADER, frag_shader_id)) {
    return;
  }
  PLOG_INFO << "OpenGL shaders compiled successfully";
  id = glCreateProgram();
  glAttachShader(id, vertex_shader_id);
  glAttachShader(id, frag_shader_id);
  glLinkProgram(id);

  check_link_result(id);

  // Delete the shader objects - we no longer need them after linking.
  glDeleteShader(vertex_shader_id);
  glDeleteShader(frag_shader_id);
}

void shader_program_opengl::activate() const { glUseProgram(id); }
// Uniforms functions
void shader_program_opengl::use_bool(const char *name, bool val) {
  glUniform1i(glGetUniformLocation(id, name), val);
}
void shader_program_opengl::use_int(const char *name, std::int32_t val) {
  glUniform1i(glGetUniformLocation(id, name), val);
}
void shader_program_opengl::use_float(const char *name, float val) {
  glUniform1f(glGetUniformLocation(id, name), val);
}

shader_program_opengl::~shader_program_opengl() { glDeleteProgram(id); }

bool shader_program_opengl::compile_shader(std::string_view source,
                                           std::int32_t shader_type,
                                           std::int32_t &out_shader_id) {
  out_shader_id = glCreateShader(shader_type);
  // Assign source code to the shader object.
  const auto str = source.data();
  const auto length = static_cast<std::int32_t>(source.size());
  glShaderSource(out_shader_id, 1, &str,
                 &length // If length is NULL, each string is assumed to
                         // be null terminated.
  );
  glCompileShader(out_shader_id);
  return check_compile_result(out_shader_id);
}

} // namespace fornix
