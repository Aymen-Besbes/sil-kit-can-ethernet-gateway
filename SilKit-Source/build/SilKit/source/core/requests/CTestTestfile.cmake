# CMake generated Testfile for 
# Source directory: /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/core/requests
# Build directory: /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/core/requests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Test_RequestReplyService "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release/SilKitUnitTests" "--gtest_output=xml:Test_RequestReplyService_gtestresults.xml" "--gtest_filter=Test_RequestReplyService.*")
set_tests_properties(Test_RequestReplyService PROPERTIES  WORKING_DIRECTORY "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release" _BACKTRACE_TRIPLES "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/cmake/SilKitTest.cmake;119;add_test;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/core/requests/CMakeLists.txt;52;add_silkit_test_to_executable;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/core/requests/CMakeLists.txt;0;")
subdirs("procs")
