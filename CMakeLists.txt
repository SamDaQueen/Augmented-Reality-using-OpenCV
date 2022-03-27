cmake_minimum_required(VERSION 3.0)
project(project4)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE-DIRECTORIES})

link_libraries(${OpenCV_LIBS})

add_subdirectory(include)
add_executable(main ./src/main.cpp)

target_link_libraries(main Calibration)
target_include_directories(main PUBLIC
"${PROJECT_BINARY_DIR}"
"${PROJECT_SOURCE_DIR}/include")

set(EXECUTABLE_OUTPUT_PATH "bin")



