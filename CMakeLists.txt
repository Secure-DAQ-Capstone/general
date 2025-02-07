cmake_minimum_required(VERSION 3.16)
project(my_project)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
message(STATUS "Protobuf Include Dir: ${Protobuf_INCLUDE_DIRS}")

# Specify the .proto file
set(PROTO_FILE_PATH our_protobuf/packet.proto)

# Generate the .cc and .h files from the .proto file
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE_PATH})

# Include the generated protobuf headers
include_directories(${CMAKE_CURRENT_BINARY_DIR})  # The generated headers will be in the build directory
# Add subdirectories for the libraries
add_subdirectory(lib/NMEA2000)
add_subdirectory(lib/NMEA2000_socketCAN)


# Define the main executable
add_executable(my_project main.cpp ${PROTO_SRCS} ${PROTO_HDRS})

# Link libraries
target_link_libraries(my_project
    PRIVATE nmea2000
    PRIVATE nmea2000_socketCAN    
    PRIVATE ${Protobuf_LIBRARIES}
)

# Include directories
target_include_directories(my_project PRIVATE
    lib/NMEA2000/src
    lib/NMEA2000_socketCAN
    ${Protobuf_INCLUDE_DIRS}
    our_protobuf/
)

