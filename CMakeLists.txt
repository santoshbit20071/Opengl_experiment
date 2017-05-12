# CMAKE for opengl experiment from learnopengl.com

cmake_minimum_required (VERSION 2.8.11)
project (openglexperment)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c++11")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
set(LINKER_FLAGS "-ldl -lGL -lXcursor -rdynamic -lX11 -lpthread -lXrandr -lXi -lXinerama")

include_directories(${CMAKE_SOURCE_DIR}/../../install/usr/include ${CMAKE_SOURCE_DIR}/../../install/include)

add_library(glfwlib STATIC IMPORTED)
set_target_properties(glfwlib PROPERTIES IMPORTED_LOCATION /home/santosh/PERSONAL/opengl_learning/install/usr/lib/libglfw3.a)

add_library(glewlib STATIC IMPORTED)
set_target_properties(glewlib PROPERTIES IMPORTED_LOCATION /home/santosh/PERSONAL/opengl_learning/install/usr/lib64/libGLEW.a)

add_library(soillib STATIC IMPORTED)
set_target_properties(soillib PROPERTIES IMPORTED_LOCATION /home/santosh/PERSONAL/opengl_learning/install/usr/lib/libSOIL.a)


Add_executable(openglexp1_texture program1.cpp)
target_link_libraries(openglexp1_texture soillib glewlib glfwlib ${LINKER_FLAGS})

Add_executable(openglexp1_transform program2.cpp)
target_link_libraries(openglexp1_transform soillib glewlib glfwlib ${LINKER_FLAGS})
