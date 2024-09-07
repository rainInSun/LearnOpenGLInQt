add_executable(glsl_01
    ${PROJECT_SOURCE_DIR}/GLSL_01/glsl_01.h
    ${PROJECT_SOURCE_DIR}/GLSL_01/glsl_01.cpp
    ${PROJECT_SOURCE_DIR}/GLSL_01/shader.h
    ${PROJECT_SOURCE_DIR}/GLSL_01/main.cpp
)

target_link_libraries(glsl_01
    PRIVATE
    Qt5::Widgets
)