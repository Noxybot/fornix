#pragma once

#include <cstdint>

namespace fornix::shaders::opengl {
// Compiles and Links all the shaders into a shader program.
bool compile_and_link_all(std::uint32_t &out_shader_program_id);
} // namespace fornix::shaders::opengl