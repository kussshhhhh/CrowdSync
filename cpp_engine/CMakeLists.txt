cmake_minimum_required(VERSION 3.10)
project(CrowdSyncGeospatial)

set(CMAKE_CXX_STANDARD 17)

# Requires nlohmann_json and cpp-httplib headers in include path
find_package(nlohmann_json REQUIRED)
find_path(HTTPLIB_INCLUDE_DIR httplib.h)

add_executable(cpp_engine main.cpp)
target_include_directories(cpp_engine PRIVATE ${HTTPLIB_INCLUDE_DIR})
target_link_libraries(cpp_engine PRIVATE nlohmann_json::nlohmann_json pthread)