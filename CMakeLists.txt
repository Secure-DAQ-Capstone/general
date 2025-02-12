cmake_minimum_required(VERSION 3.16)
project(capstone_dac)

find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
message(STATUS "Protobuf Include Dir: ${Protobuf_INCLUDE_DIRS}")

# Specify the .proto file
set(PROTO_FILE_PATH protobuf/packet.proto)

# Generate the .cc and .h files from the .proto file
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILE_PATH})

# Include the generated protobuf headers
include_directories(${CMAKE_CURRENT_BINARY_DIR})  # The generated headers will be in the build directory
include_directories(include)

# Add subdirectories for the libraries
add_subdirectory(submodules/NMEA2000)
add_subdirectory(submodules/NMEA2000_socketCAN)

add_library(libudp_pub
  lib/udp_pub.cpp
)
add_library(libudp_sub
  lib/udp_sub.cpp
)


############# NMEA READER

# Define the main executable
add_executable(nmea_reader src/nmea_reader.cpp ${PROTO_SRCS} ${PROTO_HDRS})

# Link libraries
target_link_libraries(nmea_reader
    PRIVATE nmea2000
    PRIVATE nmea2000_socketCAN    
    PRIVATE ${Protobuf_LIBRARIES}
    libudp_pub
    libudp_sub
)

############## udp_test file

# Add executable for testing pub/sub functionality
add_executable(udp_test src/test_roles.cpp)

# Link for test
target_link_libraries(udp_test
    libudp_pub
    libudp_sub
)

############# NAUTILUS Subscriber File

# Add executable for testing pub/sub functionality
add_executable(nautilus src/nautilus.cpp)

# Link for test
target_link_libraries(nautilus
    libudp_sub
)