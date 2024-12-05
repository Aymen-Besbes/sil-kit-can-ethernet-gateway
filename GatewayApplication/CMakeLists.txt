cmake_minimum_required(VERSION 3.10)  # Updated to a more recent version

project(GatewayApplication)

set(CMAKE_BUILD_TYPE Debug)

# Find the SilKit package (make sure you have the correct path to SilKitConfig.cmake)
find_package(SilKit REQUIRED CONFIG PATHS "${CMAKE_CURRENT_LIST_DIR}/../SilKit")

# Find Threads package (useful for linking with threading libraries)
find_package(Threads REQUIRED)

# Add the executable (simple.cpp is your source file)
add_executable(CanWriter canWriter.cpp )
add_executable(EthernetReader ethernet.cpp)
add_executable(Gateway canReaderEthernetWriter.cpp)
# Link SilKit and Threads libraries
target_link_libraries(EthernetReader PRIVATE SilKit::SilKit Threads::Threads)
target_link_libraries(CanWriter PRIVATE SilKit::SilKit Threads::Threads)
target_link_libraries(Gateway PRIVATE SilKit::SilKit Threads::Threads)

# copy the configuration file it to the output directory
configure_file(${CMAKE_CURRENT_LIST_DIR}/NetworkSimulatorConfig.yaml
               ${CMAKE_BINARY_DIR}/NetworkSimulatorConfig.yaml COPYONLY)

configure_file(${CMAKE_CURRENT_LIST_DIR}/Demo.silkit.yaml
               ${CMAKE_BINARY_DIR}/Demo.silkit.yaml COPYONLY)