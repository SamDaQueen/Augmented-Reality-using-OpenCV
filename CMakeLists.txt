cmake_minimum_required(VERSION 3.0)
project(project4)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE-DIRECTORIES})

link_libraries(${OpenCV_LIBS})

add_subdirectory(include)
add_executable(calibrate_camera ./src/calibrate_camera.cpp)
add_executable(create_object ./src/create_object.cpp)

target_link_libraries(calibrate_camera Calibration)
target_include_directories(calibrate_camera PUBLIC
"${PROJECT_BINARY_DIR}"
"${PROJECT_SOURCE_DIR}/include")

target_link_libraries(create_object Calibration)
target_include_directories(create_object PUBLIC
"${PROJECT_BINARY_DIR}"
"${PROJECT_SOURCE_DIR}/include")

set(EXECUTABLE_OUTPUT_PATH "bin")



