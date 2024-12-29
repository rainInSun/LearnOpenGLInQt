add_executable(openglcamera_01
    ${PROJECT_SOURCE_DIR}/OpenGLCamera_01/openglcamera_01.h
    ${PROJECT_SOURCE_DIR}/OpenGLCamera_01/openglcamera_01.cpp
    ${PROJECT_SOURCE_DIR}/OpenGLCamera_01/camera.h
    ${PROJECT_SOURCE_DIR}/OpenGLCamera_01/camera.cpp
    ${PROJECT_SOURCE_DIR}/OpenGLCamera_01/textureobject.h
    ${PROJECT_SOURCE_DIR}/OpenGLCamera_01/textureobject.cpp
    ${PROJECT_SOURCE_DIR}/OpenGLCamera_01/shaderprogram.h
    ${PROJECT_SOURCE_DIR}/OpenGLCamera_01/main.cpp
)

target_link_libraries(openglcamera_01
    PRIVATE
    Qt5::Widgets
    Qt5::OpenGL
)

install(DIRECTORY
    ${PROJECT_SOURCE_DIR}/OpenGLCamera_01/img
    DESTINATION ${CMAKE_BINARY_DIR}/OpenGLCamera_01/$<CONFIG>
)