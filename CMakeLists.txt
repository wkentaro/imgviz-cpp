cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(imgviz)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} include)
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})

add_executable(depth_to_bgr examples/depth_to_bgr.cpp)
target_link_libraries(depth_to_bgr ${OpenCV_LIBRARIES})

add_executable(centerize examples/centerize.cpp)
target_link_libraries(centerize ${OpenCV_LIBRARIES})

add_executable(tile examples/tile.cpp)
target_link_libraries(tile ${OpenCV_LIBRARIES})

add_executable(label_to_bgr examples/label_to_bgr.cpp)
target_link_libraries(label_to_bgr ${OpenCV_LIBRARIES})

add_executable(getting_started examples/getting_started.cpp)
target_link_libraries(getting_started ${OpenCV_LIBRARIES})
