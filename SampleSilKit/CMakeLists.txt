cmake_minimum_required(VERSION 3.10)  # Updated to a more recent version

project(SampleSilKit)

set(CMAKE_BUILD_TYPE Debug)

# Find the SilKit package (make sure you have the correct path to SilKitConfig.cmake)
find_package(SilKit REQUIRED CONFIG PATHS "${CMAKE_CURRENT_LIST_DIR}/SilKit")

# Find Threads package (useful for linking with threading libraries)
find_package(Threads REQUIRED)

# Add the executable (simple.cpp is your source file)
add_executable(${PROJECT_NAME} simple.cpp)

# Link SilKit and Threads libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SilKit::SilKit Threads::Threads)

# copy the configuration file it to the output directory
configure_file(${CMAKE_CURRENT_LIST_DIR}/simple.yaml
               ${CMAKE_BINARY_DIR}/simple.yaml COPYONLY)
