cmake_minimum_required(VERSION 3.15)

project(LightRT)

set(CMAKE_CXX_STANDARD 20)

set(HEADERS
    ${PROJECT_SOURCE_DIR}/include/crtcolor.h    
    ${PROJECT_SOURCE_DIR}/include/crtpoint.h
    ${PROJECT_SOURCE_DIR}/include/crtray.h
    ${PROJECT_SOURCE_DIR}/include/crttriangle.h
    ${PROJECT_SOURCE_DIR}/include/crtvector.h
    ${PROJECT_SOURCE_DIR}/include/crtmatrix.h
    ${PROJECT_SOURCE_DIR}/include/crtcamera.h
    ${PROJECT_SOURCE_DIR}/include/crtsettings.h
    ${PROJECT_SOURCE_DIR}/include/crtmesh.h
    ${PROJECT_SOURCE_DIR}/include/crtlight.h
    ${PROJECT_SOURCE_DIR}/include/crtscene.h
    ${PROJECT_SOURCE_DIR}/include/crtcanvas.h
    ${PROJECT_SOURCE_DIR}/include/crtrenderer.h)

set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/crtvector.cpp
    ${PROJECT_SOURCE_DIR}/src/crtmatrix.cpp
    ${PROJECT_SOURCE_DIR}/src/crtmesh.cpp
    ${PROJECT_SOURCE_DIR}/src/crtcamera.cpp
    ${PROJECT_SOURCE_DIR}/src/crtscene.cpp
    ${PROJECT_SOURCE_DIR}/src/crtcanvas.cpp
    ${PROJECT_SOURCE_DIR}/src/crtrenderer.cpp)

add_subdirectory(app)

include(FetchContent)

FetchContent_Declare(
    rapidjson
    URL https://github.com/Tencent/rapidjson/archive/refs/tags/v1.1.0.zip
)
FetchContent_MakeAvailable(rapidjson)

FetchContent_Declare(
    threadpool
    URL https://github.com/bshoshany/thread-pool/archive/refs/tags/v2.0.0.zip
)
FetchContent_MakeAvailable(threadpool)