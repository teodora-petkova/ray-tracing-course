cmake_minimum_required(VERSION 3.15)

project(LightRT_app)

add_executable(${PROJECT_NAME} 
    main.cpp    
    ${HEADERS}
    ${SOURCES}
)

find_package(rapidjson)
include_directories(${FETCHCONTENT_BASE_DIR}/rapidjson-src/include)

include_directories(${FETCHCONTENT_BASE_DIR}/threadpool-src/)