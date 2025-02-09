add_executable(lighting_01
    ${PROJECT_SOURCE_DIR}/Lighting_01/lighting_01.h
    ${PROJECT_SOURCE_DIR}/Lighting_01/lighting_01.cpp
    ${PROJECT_SOURCE_DIR}/Lighting_01/camera.h
    ${PROJECT_SOURCE_DIR}/Lighting_01/camera.cpp
    ${PROJECT_SOURCE_DIR}/Lighting_01/shaderprogram.h
    ${PROJECT_SOURCE_DIR}/Lighting_01/main.cpp
)

target_link_libraries(lighting_01
    PRIVATE
    Qt5::Widgets
    Qt5::OpenGL
)