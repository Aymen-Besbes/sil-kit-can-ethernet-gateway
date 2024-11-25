# CMake generated Testfile for 
# Source directory: /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/tracing
# Build directory: /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/tracing
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(Test_Pcap "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release/SilKitUnitTests" "--gtest_output=xml:Test_Pcap_gtestresults.xml" "--gtest_filter=Test_Pcap.*")
set_tests_properties(Test_Pcap PROPERTIES  WORKING_DIRECTORY "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release" _BACKTRACE_TRIPLES "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/cmake/SilKitTest.cmake;119;add_test;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/tracing/CMakeLists.txt;98;add_silkit_test_to_executable;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/tracing/CMakeLists.txt;0;")
add_test(Test_EthernetReplay "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release/SilKitUnitTests" "--gtest_output=xml:Test_EthernetReplay_gtestresults.xml" "--gtest_filter=Test_EthernetReplay.*")
set_tests_properties(Test_EthernetReplay PROPERTIES  WORKING_DIRECTORY "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/Release" _BACKTRACE_TRIPLES "/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/cmake/SilKitTest.cmake;119;add_test;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/tracing/CMakeLists.txt;99;add_silkit_test_to_executable;/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/tracing/CMakeLists.txt;0;")
