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

set(CMAKE_PREFIX_PATH "/home/gracepearcey/.programs/libsodium-stable/zig-out" ${CMAKE_PREFIX_PATH})
set(CMAKE_LIBRARY_PATH "/home/gracepearcey/.programs/libsodium-stable/zig-out/lib" ${CMAKE_LIBRARY_PATH})
set(CMAKE_INCLUDE_PATH "/home/gracepearcey/.programs/libsodium-stable/zig-out/include" ${CMAKE_INCLUDE_PATH})
set(ENV{PKG_CONFIG_PATH} "/home/gracepearcey/.programs/libsodium-stable/zig-out/lib/pkgconfig")


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/lib")
find_package(Sodium REQUIRED)
include_directories("/home/gracepearcey/.programs/libsodium-stable/zig-out/include")

message(STATUS "Sodium Libraries: ${sodium_LIBRARIES}")
find_path(SODIUM_INCLUDE_DIR sodium.h
    HINTS /home/gracepearcey/.programs/libsodium-stable/zig-out/include
)
include_directories(${SODIUM_INCLUDE_DIR})
message(STATUS "Sodium Include: ${sodium_INCLUDE_DIR}")


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
find_package(PkgConfig)
pkg_check_modules(MONGOCXX libmongocxx)
pkg_check_modules(BSONCXX libbsoncxx)

# Enable MongoDB support only if both libraries are found
if (MONGOCXX_FOUND AND BSONCXX_FOUND)
    message(STATUS "MongoDB libraries found. Enabling MongoDB support.")
    set(USE_MONGO ON)
else()
    message(WARNING "MongoDB libraries NOT found. Disabling MongoDB support.")
    set(USE_MONGO OFF)
endif()


# Parameters
set(USE_BROADCAST OFF)



add_library(security STATIC
  lib/security.cpp
)

target_link_libraries(security
  PRIVATE ${sodium_LIBRARIES}
  Sodium
)

############# NMEA READER


# Set the WASI SDK path (update this if your path is different)
set(WASI_SDK_PATH $ENV{WASI_SDK_PATH})

# Set the C++ compiler to WASI Clang++
set(CMAKE_CXX_COMPILER "${WASI_SDK_PATH}/bin/clang++")

# Set necessary flags
set(CMAKE_CXX_FLAGS "--sysroot=${WASI_SDK_PATH}/share/wasi-sysroot/")

# Include Wasmtime headers
include_directories(/home/gracepearcey/.programs/wasmtime/target/release/build/wasmtime-c-api-impl-8f8a571fe5e24873/out/include)

# # Define the executable
# add_executable(wasi.wasm wasi.cpp)

# Define the main executable
add_executable(nmea_reader src/nmea_reader.cpp ${PROTO_SRCS} ${PROTO_HDRS})

# Link libraries
target_link_libraries(nmea_reader
    PRIVATE nmea2000
    PRIVATE nmea2000_socketCAN    
    PRIVATE ${Protobuf_LIBRARIES}
    libudp_pub
    libudp_sub
    security
)

############## udp_test file

# Add executable for testing pub/sub functionality
add_executable(udp_test src/test_roles.cpp)

# Link for test
target_link_libraries(udp_test
    libudp_pub
    libudp_sub
)

############# MongoApp File

if (USE_MONGO)

  add_library(db_handler
  lib/mongodb_handler.cpp
  )

  add_library(proto_json
  lib/proto_json_converter.cpp
  )

  #Include MongoDB headers and libraries for db_handler
  target_include_directories(db_handler PRIVATE ${MONGOCXX_INCLUDE_DIRS} ${BSONCXX_INCLUDE_DIRS})
  target_link_directories(db_handler PRIVATE ${MONGOCXX_LIBRARY_DIRS} ${BSONCXX_LIBRARY_DIRS})
  target_link_libraries(db_handler PRIVATE ${MONGOCXX_LIBRARIES} ${BSONCXX_LIBRARIES})
  target_compile_options(db_handler PRIVATE ${MONGOCXX_CFLAGS_OTHER} ${BSONCXX_CFLAGS_OTHER})

  #Find and link MongoDB libraries
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(MONGOCXX REQUIRED libmongocxx)
  pkg_check_modules(BSONCXX REQUIRED libbsoncxx)

  #Add executable for testing pub/sub functionality
  add_executable(mongo_app src/mongo_app.cpp ${PROTO_SRCS} ${PROTO_HDRS})


  #Include MongoDB headers and libraries
  target_include_directories(mongo_app PRIVATE ${MONGOCXX_INCLUDE_DIRS} ${BSONCXX_INCLUDE_DIRS})
  target_link_directories(mongo_app PRIVATE ${MONGOCXX_LIBRARY_DIRS} ${BSONCXX_LIBRARY_DIRS})
  target_link_libraries(mongo_app
    PRIVATE ${Protobuf_LIBRARIES}
    libudp_sub
    db_handler
    application
    security
    proto_json
    constants
    PRIVATE ${MONGOCXX_LIBRARIES} ${BSONCXX_LIBRARIES}
  )

  target_compile_options(mongo_app PRIVATE ${MONGOCXX_CFLAGS_OTHER} ${BSONCXX_CFLAGS_OTHER})
  set(CMAKE_INSTALL_RPATH "${MONGOCXX_LIBRARY_DIRS};${BSONCXX_LIBRARY_DIRS}")

endif()


##### Broadcast

if (USE_BROADCAST)

  add_executable(receiver src/broadcast_receiver.cpp)

  add_executable(sender src/broadcast_sender.cpp)

endif()


############# main Application File

#Add executable
add_executable(app_main src/app_main.cpp ${PROTO_SRCS} ${PROTO_HDRS})

add_library(application
  lib/application.cpp
  )

add_library(constants
  lib/constants.cpp
  )

target_link_libraries(app_main
  PRIVATE ${Protobuf_LIBRARIES}
  libudp_sub
  application
  security
  constants
)


############# Relay File

#Add executable
add_executable(relay_main src/relay_main.cpp ${PROTO_SRCS} ${PROTO_HDRS})


add_library(relay
  lib/relay.cpp
  )

target_link_libraries(relay_main
  PRIVATE ${Protobuf_LIBRARIES}
  libudp_sub
  relay
  application
  security
  constants
)