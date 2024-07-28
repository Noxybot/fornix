set(SOURCES
    src/shaders.cpp)

add_library(shaders ${SOURCES})

target_link_libraries(shaders
    PRIVATE
    plog)
target_include_directories(shaders
    PUBLIC
    include/
    PRIVATE
    ${GLFW_SOURCE_DIR}/deps
)
