cmake_minimum_required(VERSION 3.21)

project(savant LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add the library subdirectory
add_subdirectory(cpp)

# main executable (for testing/demo purposes)
add_executable(savant_test main.cpp)
# link against the savant library if needed
target_link_libraries(savant_test PRIVATE savant)