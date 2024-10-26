add_executable(scenetransition
    ${PROJECT_SOURCE_DIR}/SceneTransition/bowtiehorizshader.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/cubeshader.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/doorwayshader.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/fadeshader.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/gridflipshader.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/luminancemeltshader.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/perlinshader.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/scenetransition.cpp
    ${PROJECT_SOURCE_DIR}/SceneTransition/scenetransition.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/scenetransitiondef.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/shaderprogrammanager.cpp
    ${PROJECT_SOURCE_DIR}/SceneTransition/shaderprogrammanager.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/shadersource.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/texturemanager.cpp
    ${PROJECT_SOURCE_DIR}/SceneTransition/texturemanager.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/waterdropshader.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/windowsliceshader.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/wipeshader.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/scenetransitionexample.cpp
    ${PROJECT_SOURCE_DIR}/SceneTransition/scenetransitionexample.h
    ${PROJECT_SOURCE_DIR}/SceneTransition/main.cpp
)

target_link_libraries(scenetransition
    PRIVATE
    Qt5::Widgets
)

install(DIRECTORY
    ${PROJECT_SOURCE_DIR}/SceneTransition/img
    DESTINATION ${CMAKE_BINARY_DIR}/SceneTransition/$<CONFIG>
)