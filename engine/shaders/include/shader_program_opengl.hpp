#pragma once
#include "shader_program.hpp"

#include <filesystem>

namespace fornix {
class shader_program_opengl : public shader_program {
  std::int32_t id = -1;

public:
  explicit shader_program_opengl(const std::filesystem::path &vertex_shader,
                                 const std::filesystem::path &fragment_shader);

  void activate() const override;
  // Uniforms functions
  void use_bool(const char *name, bool val) override;
  void use_int(const char *name, std::int32_t val) override;
  void use_float(const char *name, float val) override;

  ~shader_program_opengl() override;

private:
  bool compile_shader(std::string_view source, std::int32_t shader_type,
                      std::int32_t &out_shader_id);
};
} // namespace fornix
