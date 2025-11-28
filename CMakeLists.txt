cmake_minimum_required(VERSION 3.31)
project(GLProject)

set(CMAKE_CXX_STANDARD 20)

include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/headers)
#include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

file(GLOB files demo/*.cpp)
foreach (file ${files})
    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" file_name ${file})
    add_executable(${file_name}
            src/glad.c
            src/object.cpp
            src/stb_image_implementation.cpp
            src/model.cpp
            src/camera.cpp
            src/shader.cpp
            src/my_math.cpp
            ${file} )
    target_link_libraries(${file_name} ${PROJECT_SOURCE_DIR}/lib/glfw3.lib)
    target_link_libraries(${file_name} ${PROJECT_SOURCE_DIR}/lib/assimp-vc143-mt.lib)
endforeach ()
