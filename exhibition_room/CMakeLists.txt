cmake_minimum_required(VERSION 2.8)

project(exhibition_room)

add_executable(exhibition_room.out main.cpp ray.cpp scene_object.cpp sphere.cpp cylinder.cpp plane.cpp textureBMP.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm REQUIRED)
set (CMAKE_CXX_FLAGS "${CMAKE_FXX_FLAGS} -pthread -std=c++17")
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLM_INCLUDE_DIR} )

target_link_libraries( exhibition_room.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLM_LIBRARY} )

