#pragma once

#include <cstdint>

namespace fornix {
class shader_program {
public:
  virtual ~shader_program() = default;
  virtual void activate() const = 0;
  // Uniforms functions
  virtual void use_bool(const char *name, bool val) = 0;
  virtual void use_int(const char *name, std::int32_t val) = 0;
  virtual void use_float(const char *name, float val) = 0;
};
} // namespace fornix
