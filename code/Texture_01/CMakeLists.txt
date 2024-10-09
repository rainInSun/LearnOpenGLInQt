add_executable(texture_01
    ${PROJECT_SOURCE_DIR}/Texture_01/texture_01.h
    ${PROJECT_SOURCE_DIR}/Texture_01/texture_01.cpp
    ${PROJECT_SOURCE_DIR}/Texture_01/textureobject.h
    ${PROJECT_SOURCE_DIR}/Texture_01/textureobject.cpp
    ${PROJECT_SOURCE_DIR}/Texture_01/shaderprogram.h
    ${PROJECT_SOURCE_DIR}/Texture_01/main.cpp
)

target_link_libraries(texture_01
    PRIVATE
    Qt5::Widgets
    Qt5::OpenGL
)

install(DIRECTORY
    ${PROJECT_SOURCE_DIR}/Texture_01/img
    DESTINATION ${CMAKE_BINARY_DIR}/Texture_01/$<CONFIG>
)