cmake_minimum_required(VERSION 3.15)

project(CameraRT_app)

add_executable(${PROJECT_NAME} 
    main.cpp    
    ${HEADERS}
    ${SOURCES}
)