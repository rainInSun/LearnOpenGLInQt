add_executable(coolprogress
    ${PROJECT_SOURCE_DIR}/CoolProgress/coolprogress.h
    ${PROJECT_SOURCE_DIR}/CoolProgress/coolprogress.cpp
    ${PROJECT_SOURCE_DIR}/CoolProgress/progressexample.h
    ${PROJECT_SOURCE_DIR}/CoolProgress/progressexample.cpp
    ${PROJECT_SOURCE_DIR}/CoolProgress/shaderprogram.h
    ${PROJECT_SOURCE_DIR}/CoolProgress/progressshader.h
    ${PROJECT_SOURCE_DIR}/CoolProgress/main.cpp
)

target_link_libraries(coolprogress
    PRIVATE
    Qt5::Widgets
)