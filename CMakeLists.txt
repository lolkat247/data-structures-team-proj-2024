cmake_minimum_required(VERSION 3.10)

project(AirportSystem VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(AirportSystem main.cpp Graph.cpp Airport.h Edge.h Graph.h Graph.cpp Main.cpp)

# Copy airports.csv to the build directory after building
add_custom_command(
        TARGET AirportSystem POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/airports.csv
        ${CMAKE_BINARY_DIR}/airports.csv
)