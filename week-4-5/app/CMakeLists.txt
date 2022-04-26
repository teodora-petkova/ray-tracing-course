cmake_minimum_required(VERSION 3.15)

project(TriangleRT_app)

add_executable(${PROJECT_NAME} 
    main.cpp    
    ${HEADERS}
    ${SOURCES}
)