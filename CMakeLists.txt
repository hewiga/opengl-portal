cmake_minimum_required(VERSION 3.29)
project(Portals)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories( 
    ${CMAKE_SOURCE_DIR}/Dependencies/GLEW/include  
    ${CMAKE_SOURCE_DIR}/Dependencies/GLFW/include 
    ${CMAKE_SOURCE_DIR}/Dependencies/glm
)

link_directories(
    ${CMAKE_SOURCE_DIR}/Dependencies/GLFW/lib  
    ${CMAKE_SOURCE_DIR}/Dependencies/GLEW/lib/Release/Win32 
)

add_compile_definitions(GLEW_STATIC)

file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} 
    glfw3
    glew32s
    opengl32
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/res"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/res"
)