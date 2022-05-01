cmake_minimum_required(VERSION 3.15)

project(CameraRT)


set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/color.h    
    ${PROJECT_SOURCE_DIR}/include/crtpoint.h
    ${PROJECT_SOURCE_DIR}/include/crtray.h
    ${PROJECT_SOURCE_DIR}/include/crttriangle.h
    ${PROJECT_SOURCE_DIR}/include/crtvector.h
    ${PROJECT_SOURCE_DIR}/include/crtmatrix.h
    ${PROJECT_SOURCE_DIR}/include/crtcamera.h)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/crtvector.cpp
    ${PROJECT_SOURCE_DIR}/src/crtmatrix.cpp
    ${PROJECT_SOURCE_DIR}/src/crtcamera.cpp)

add_subdirectory(app)
add_subdirectory(test)