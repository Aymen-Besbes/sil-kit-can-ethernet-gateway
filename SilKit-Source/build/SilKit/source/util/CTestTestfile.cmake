# CMake generated Testfile for 
# Source directory: /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/util
# Build directory: /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/util
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Test_LabelMatching "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release/SilKitUnitTests" "--gtest_output=xml:Test_LabelMatching_gtestresults.xml" "--gtest_filter=Test_LabelMatching.*")
set_tests_properties(Test_LabelMatching PROPERTIES  WORKING_DIRECTORY "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release" _BACKTRACE_TRIPLES "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/cmake/SilKitTest.cmake;119;add_test;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/util/CMakeLists.txt;120;add_silkit_test_to_executable;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/util/CMakeLists.txt;0;")
subdirs("tests")
