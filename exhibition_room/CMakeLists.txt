cmake_minimum_required(VERSION 2.8)

project(ExhibitionRoom)

add_executable(ExhibitionRoom.out RayTracer.cpp Ray.cpp SceneObject.cpp Sphere.cpp MyCylinder.cpp Plane.cpp TextureBMP.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glm REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${GLM_INCLUDE_DIR} )

target_link_libraries( ExhibitionRoom.out ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${GLM_LIBRARY} )

