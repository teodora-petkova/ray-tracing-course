cmake_minimum_required(VERSION 3.15)

project(TriangleRT)


set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/color.h    
    ${PROJECT_SOURCE_DIR}/include/crtpoint.h
    ${PROJECT_SOURCE_DIR}/include/crtray.h
    ${PROJECT_SOURCE_DIR}/include/crttriangle.h
    ${PROJECT_SOURCE_DIR}/include/crtvector.h
)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/crtray.cpp
    ${PROJECT_SOURCE_DIR}/src/crtvector.cpp)

add_subdirectory(app)
add_subdirectory(test)