cmake_minimum_required(VERSION 3.13)
project(thread_pool)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(SRC_FILES example/main.cpp)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} pthread)
