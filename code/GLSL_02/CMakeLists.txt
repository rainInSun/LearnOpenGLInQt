add_executable(glsl_02
    ${PROJECT_SOURCE_DIR}/GLSL_02/glsl_02.h
    ${PROJECT_SOURCE_DIR}/GLSL_02/glsl_02.cpp
    ${PROJECT_SOURCE_DIR}/GLSL_02/main.cpp
)

target_link_libraries(glsl_02
    PRIVATE
    Qt5::Widgets
)