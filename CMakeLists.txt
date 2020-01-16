cmake_minimum_required(VERSION 3.15)
project(TeamProject4)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(include)
include_directories(include/chipmunk)
include_directories(include/glm)
include_directories(include/glm/detail)
include_directories(include/glm/ext)
include_directories(include/glm/gtc)
include_directories(include/glm/gtx)
include_directories(include/glm/simd)

find_library(CocoaLib Cocoa)
find_library(OpenGLLib OpenGL)
find_library(IOKitLib IOKit)
find_library(CoreVideoLib CoreVideo)

set(Frameworks
        ${CocoaLib}
        ${OpenGLLib}
        ${IOKitLib}
        ${CoreVideoLib})

add_executable(TeamProject4
        ConfigurationManager.cpp
        ConfigurationManager.h
        ECS.cpp
        ECS.h
        GameScene.cpp
        GameScene.h
        Helper.h
        main.cpp
        Primitives.cpp
        Primitives.h
        Window.cpp
        Window.h CircleCollider.cpp CircleCollider.h)

target_link_libraries(TeamProject4 PUBLIC ${Frameworks})
target_link_libraries(TeamProject4 PUBLIC ${PROJECT_SOURCE_DIR}/lib/libglfw3.a)
target_link_libraries(TeamProject4 PUBLIC ${PROJECT_SOURCE_DIR}/lib/libraylib.a)