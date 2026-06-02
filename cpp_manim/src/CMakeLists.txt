add_library(manimcpp
    constants.cpp
    utils.cpp
    mobject.cpp
    scene.cpp
    camera.cpp
    animation.cpp
    geometry.cpp
    opengl.cpp
)

target_include_directories(manimcpp PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(manimcpp
    glfw
    GLEW
    ${OPENGL_LIBRARIES}
    fmt
    spdlog
)

install(TARGETS manimcpp
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/manimcpp
    DESTINATION include
)