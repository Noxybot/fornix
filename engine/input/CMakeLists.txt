set(SOURCES
    src/process_input.cpp)

add_library(input ${SOURCES})

target_link_libraries(input
    PRIVATE
    glfw)

target_include_directories(input
    PUBLIC
    include/
)
