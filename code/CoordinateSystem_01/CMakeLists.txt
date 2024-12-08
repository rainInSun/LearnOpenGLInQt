add_executable(coordinatesystem_01
    ${PROJECT_SOURCE_DIR}/CoordinateSystem_01/coordinatesystem_01.h
    ${PROJECT_SOURCE_DIR}/CoordinateSystem_01/coordinatesystem_01.cpp
    ${PROJECT_SOURCE_DIR}/CoordinateSystem_01/textureobject.h
    ${PROJECT_SOURCE_DIR}/CoordinateSystem_01/textureobject.cpp
    ${PROJECT_SOURCE_DIR}/CoordinateSystem_01/shaderprogram.h
    ${PROJECT_SOURCE_DIR}/CoordinateSystem_01/main.cpp
)

target_link_libraries(coordinatesystem_01
    PRIVATE
    Qt5::Widgets
    Qt5::OpenGL
)

install(DIRECTORY
    ${PROJECT_SOURCE_DIR}/CoordinateSystem_01/img
    DESTINATION ${CMAKE_BINARY_DIR}/CoordinateSystem_01/$<CONFIG>
)