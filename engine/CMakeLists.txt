add_executable(engine main.cpp)

target_include_directories(engine
    PRIVATE
    ${GLFW_SOURCE_DIR}/deps
)

target_link_libraries(engine
    PRIVATE
    ${OPENGL_LIBRARIES}
    glfw
    plog
)

install(TARGETS engine DESTINATION bin)
