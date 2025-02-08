# filepath: /Users/matthewgrouchy/projects/capstone/nmea_test/lib/udp_lib/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Define the library
add_library(udp_lib STATIC
    udp-pub.cpp
)

# Specify include directories
target_include_directories(udp_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link any necessary libraries (if needed)
# target_link_libraries(udp_lib <other_libraries>)