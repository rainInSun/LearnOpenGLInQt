cmake_minimum_required(VERSION 3.18)
project(LearnOpenGLInQt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
endif()

set(PROJECT_VERSION "1.0.0")

if(NOT DEFINED QT_INSTALL_PATH)
	message(FATAL_ERROR "please set Qt install dir to QT_PATH")
endif()

set(Qt5_DIR ${QT_INSTALL_PATH})
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)


add_subdirectory(Hello_Triangle_01)
add_subdirectory(GLSL_01)
add_subdirectory(GLSL_02)
add_subdirectory(CoolProgress)
add_subdirectory(Texture_01)
add_subdirectory(SceneTransition)
add_subdirectory(CoordinateSystem_01)
add_subdirectory(OpenGLCamera_01)
add_subdirectory(Lighting_01)

# cmake like this
# cmake -G"Visual Studio 15 2017" -AWin32 -BDebug -Hcode -DCMAKE_BUILD_TYPE=Debug -DQT_INSTALL_PATH="F:\Qt\5.12.5\msvc2017\lib\cmake\Qt5"
# msbuild /m LearnOpenGLInQt.sln /p:Platform="Win32" /p:Configuration="Debug"
# cmake --install . --config Debug