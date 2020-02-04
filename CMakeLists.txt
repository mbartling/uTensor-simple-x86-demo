cmake_minimum_required(VERSION 3.10)

project(utensor-demo)
add_subdirectory(uTensor)

# Hack, handling no-RTTI is a pain but needed for embedded systems
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-rtti")

add_executable(demo main.cpp models/MyModel.cpp)
target_include_directories(demo PRIVATE models)

target_link_libraries(demo utensor)
target_compile_features(demo PUBLIC cxx_std_11)
