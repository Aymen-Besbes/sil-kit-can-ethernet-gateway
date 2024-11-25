# CMake generated Testfile for 
# Source directory: /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/core/internal
# Build directory: /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/core/internal
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Test_MessageBuffer "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release/SilKitUnitTests" "--gtest_output=xml:Test_MessageBuffer_gtestresults.xml" "--gtest_filter=Test_MessageBuffer.*")
set_tests_properties(Test_MessageBuffer PROPERTIES  WORKING_DIRECTORY "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release" _BACKTRACE_TRIPLES "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/cmake/SilKitTest.cmake;119;add_test;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/core/internal/CMakeLists.txt;50;add_silkit_test_to_executable;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/core/internal/CMakeLists.txt;0;")
add_test(Test_InternalSerdes "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release/SilKitUnitTests" "--gtest_output=xml:Test_InternalSerdes_gtestresults.xml" "--gtest_filter=Test_InternalSerdes.*")
set_tests_properties(Test_InternalSerdes PROPERTIES  WORKING_DIRECTORY "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release" _BACKTRACE_TRIPLES "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/cmake/SilKitTest.cmake;119;add_test;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/core/internal/CMakeLists.txt;51;add_silkit_test_to_executable;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/core/internal/CMakeLists.txt;0;")
