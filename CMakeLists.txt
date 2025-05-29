cmake_minimum_required(VERSION 3.10)
project(SolitareGigathon_v1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

if(WIN32)
    include_directories(${CMAKE_SOURCE_DIR}/include/pdcursesmod)
    target_link_libraries(${PROJECT_NAME} 
        ${CMAKE_SOURCE_DIR}/lib/pdcurses.lib
        winmm
    )
else()
    target_link_libraries(${PROJECT_NAME} ncurses)
endif()
