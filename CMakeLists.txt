cmake_minimum_required(VERSION 3.16)
project(capstone_dac)

set(CMAKE_CXX_STANDARD 17)

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

# Find and link MongoDB libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(MONGOCXX REQUIRED libmongocxx)
pkg_check_modules(BSONCXX REQUIRED libbsoncxx)

add_library(db_handler
  lib/mongodb_handler.cpp
)

# Include MongoDB headers and libraries for db_handler
target_include_directories(db_handler PRIVATE ${MONGOCXX_INCLUDE_DIRS} ${BSONCXX_INCLUDE_DIRS})
target_link_directories(db_handler PRIVATE ${MONGOCXX_LIBRARY_DIRS} ${BSONCXX_LIBRARY_DIRS})
target_link_libraries(db_handler PRIVATE ${MONGOCXX_LIBRARIES} ${BSONCXX_LIBRARIES})
target_compile_options(db_handler PRIVATE ${MONGOCXX_CFLAGS_OTHER} ${BSONCXX_CFLAGS_OTHER})

add_library(proto_json
  lib/proto_json_converter.cpp
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

# Find and link MongoDB libraries
find_package(PkgConfig REQUIRED)
pkg_check_modules(MONGOCXX REQUIRED libmongocxx)
pkg_check_modules(BSONCXX REQUIRED libbsoncxx)

# Add executable for testing pub/sub functionality
add_executable(application src/application.cpp ${PROTO_SRCS} ${PROTO_HDRS})


# Include MongoDB headers and libraries
target_include_directories(application PRIVATE ${MONGOCXX_INCLUDE_DIRS} ${BSONCXX_INCLUDE_DIRS})
target_link_directories(application PRIVATE ${MONGOCXX_LIBRARY_DIRS} ${BSONCXX_LIBRARY_DIRS})
target_link_libraries(application
    PRIVATE ${Protobuf_LIBRARIES}
    libudp_sub
    db_handler
    proto_json
    PRIVATE ${MONGOCXX_LIBRARIES} ${BSONCXX_LIBRARIES}
)

target_compile_options(application PRIVATE ${MONGOCXX_CFLAGS_OTHER} ${BSONCXX_CFLAGS_OTHER})
set(CMAKE_INSTALL_RPATH "${MONGOCXX_LIBRARY_DIRS};${BSONCXX_LIBRARY_DIRS}")

##### Broadcast

add_executable(receiver src/broadcast_receiver.cpp)

add_executable(sender src/broadcast_sender.cpp)




