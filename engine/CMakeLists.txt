add_subdirectory(input)
add_subdirectory(shaders)

add_executable(engine main.cpp)

target_include_directories(engine
    PRIVATE
    ${GLFW_SOURCE_DIR}/deps
)

target_link_libraries(engine
    PRIVATE
    input
    shaders

    # 3rd party.
    ${OPENGL_LIBRARIES}
    glfw
    plog
)

install(TARGETS engine DESTINATION bin)
