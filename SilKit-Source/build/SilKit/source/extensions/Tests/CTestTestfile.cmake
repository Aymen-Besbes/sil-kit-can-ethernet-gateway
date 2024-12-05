# CMake generated Testfile for 
# Source directory: /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/extensions/Tests
# Build directory: /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/extensions/Tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Test_SilKitExtensions "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release/SilKitUnitTests" "--gtest_output=xml:Test_SilKitExtensions_gtestresults.xml" "--gtest_filter=Test_SilKitExtensions.*")
set_tests_properties(Test_SilKitExtensions PROPERTIES  WORKING_DIRECTORY "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release" _BACKTRACE_TRIPLES "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/cmake/SilKitTest.cmake;119;add_test;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/extensions/Tests/CMakeLists.txt;55;add_silkit_test_to_executable;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/extensions/Tests/CMakeLists.txt;0;")
