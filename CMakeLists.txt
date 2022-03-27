cmake_minimum_required(VERSION 3.0)
project(project4)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE-DIRECTORIES})

link_libraries(${OpenCV_LIBS})

add_executable(calibrate ./src/calibrate.cpp)

set(EXECUTABLE_OUTPUT_PATH "bin")



