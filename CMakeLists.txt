cmake_minimum_required(VERSION 3.16)
project(my_project)

# Add subdirectories for the libraries
add_subdirectory(lib/NMEA2000)
add_subdirectory(lib/NMEA2000_socketCAN)

# Define the main executable
add_executable(my_project main.cpp)

# Link libraries
target_link_libraries(my_project
    PRIVATE nmea2000
    PRIVATE nmea2000_socketCAN
)

# Include directories
target_include_directories(my_project PRIVATE
    lib/NMEA2000/src
    lib/NMEA2000_socketCAN
)
