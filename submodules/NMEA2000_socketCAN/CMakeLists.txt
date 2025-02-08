add_library(
    nmea2000_socketCAN
    NMEA2000_SocketCAN.cpp
    NMEA2000_SocketCAN.h
)

target_link_libraries(nmea2000_socketCAN PUBLIC nmea2000)
target_include_directories(nmea2000_socketCAN PUBLIC "${CMAKE_SOURCE_DIR}/lib/NMEA2000/src")
target_include_directories(nmea2000_socketCAN PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
