cmake_minimum_required(VERSION 3.10)  # Updated to a more recent version

project(CanGateway)

set(CMAKE_BUILD_TYPE Debug)

# Find the SilKit package (make sure you have the correct path to SilKitConfig.cmake)
find_package(SilKit REQUIRED CONFIG PATHS "${CMAKE_CURRENT_LIST_DIR}/../SilKit")

# Find Threads package (useful for linking with threading libraries)
find_package(Threads REQUIRED)

# Add the executable (simple.cpp is your source file)
add_executable(${PROJECT_NAME} gateway.cpp ethernetReader.cpp)

# Link SilKit and Threads libraries
target_link_libraries(${PROJECT_NAME} PRIVATE SilKit::SilKit Threads::Threads)

# copy the configuration files it to the output directory
configure_file(${CMAKE_CURRENT_LIST_DIR}/NetworkSimulatorConfig.yaml
               ${CMAKE_BINARY_DIR}/NetworkSimulatorConfig.yaml COPYONLY)

configure_file(${CMAKE_CURRENT_LIST_DIR}/DemoCan.silkit.yaml
               ${CMAKE_BINARY_DIR}/DemoCan.silkit.yaml COPYONLY)