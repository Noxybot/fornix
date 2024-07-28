cmake_minimum_required(VERSION 3.23)

project(fornix)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "Install path prefix, prepended onto install directories." FORCE)

# We need to include thirdparty first since other libs might depend on the variables defined by it.
add_subdirectory(thirdparty)
add_subdirectory(engine)
