add_executable(hellotriangle_01
    ${PROJECT_SOURCE_DIR}/Hello_Triangle/hellotriangle_01.h
    ${PROJECT_SOURCE_DIR}/Hello_Triangle/hellotriangle_01.cpp
    ${PROJECT_SOURCE_DIR}/Hello_Triangle/main.cpp
)

target_link_libraries(hellotriangle_01
    PRIVATE
    Qt5::Widgets
)