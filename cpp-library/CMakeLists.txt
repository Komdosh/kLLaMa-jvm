cmake_minimum_required(VERSION 3.31.0)
project(native-lib)

set(CMAKE_CXX_STANDARD 23)

add_library(native-lib SHARED src/main/cpp/native-lib.cpp)

# JNI headers
find_package(JNI REQUIRED)
include_directories(${JNI_INCLUDE_DIRS})

# Export symbols for JNI
set_target_properties(native-lib PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_VISIBILITY_PRESET default
)