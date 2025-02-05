cmake_minimum_required(VERSION 3.16)
project(my_project)

find_package(Protobuf REQUIRED)

# Add subdirectories for the libraries
add_subdirectory(lib/NMEA2000)
add_subdirectory(lib/NMEA2000_socketCAN)

set(PROTO_SRCS our_protobuf/packet.pb.cc)
set(PROTO_HDRS our_protobuf/packet.pb.h)

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

