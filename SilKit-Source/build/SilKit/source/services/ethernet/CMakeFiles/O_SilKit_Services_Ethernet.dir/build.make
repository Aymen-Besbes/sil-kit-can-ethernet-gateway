# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build

# Include any dependencies generated for this target.
include SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/depend.make

# Include the progress variables for this target.
include SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/progress.make

# Include the compile flags for this target's objects.
include SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/flags.make

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/EthController.cpp.o: SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/flags.make
SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/EthController.cpp.o: ../SilKit/source/services/ethernet/EthController.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/EthController.cpp.o"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/O_SilKit_Services_Ethernet.dir/EthController.cpp.o -c /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/EthController.cpp

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/EthController.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/O_SilKit_Services_Ethernet.dir/EthController.cpp.i"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/EthController.cpp > CMakeFiles/O_SilKit_Services_Ethernet.dir/EthController.cpp.i

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/EthController.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/O_SilKit_Services_Ethernet.dir/EthController.cpp.s"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/EthController.cpp -o CMakeFiles/O_SilKit_Services_Ethernet.dir/EthController.cpp.s

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehavior.cpp.o: SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/flags.make
SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehavior.cpp.o: ../SilKit/source/services/ethernet/SimBehavior.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehavior.cpp.o"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehavior.cpp.o -c /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/SimBehavior.cpp

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehavior.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehavior.cpp.i"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/SimBehavior.cpp > CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehavior.cpp.i

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehavior.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehavior.cpp.s"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/SimBehavior.cpp -o CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehavior.cpp.s

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorDetailed.cpp.o: SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/flags.make
SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorDetailed.cpp.o: ../SilKit/source/services/ethernet/SimBehaviorDetailed.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorDetailed.cpp.o"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorDetailed.cpp.o -c /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/SimBehaviorDetailed.cpp

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorDetailed.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorDetailed.cpp.i"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/SimBehaviorDetailed.cpp > CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorDetailed.cpp.i

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorDetailed.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorDetailed.cpp.s"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/SimBehaviorDetailed.cpp -o CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorDetailed.cpp.s

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorTrivial.cpp.o: SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/flags.make
SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorTrivial.cpp.o: ../SilKit/source/services/ethernet/SimBehaviorTrivial.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorTrivial.cpp.o"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorTrivial.cpp.o -c /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/SimBehaviorTrivial.cpp

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorTrivial.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorTrivial.cpp.i"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/SimBehaviorTrivial.cpp > CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorTrivial.cpp.i

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorTrivial.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorTrivial.cpp.s"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/SimBehaviorTrivial.cpp -o CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorTrivial.cpp.s

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/EthernetSerdes.cpp.o: SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/flags.make
SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/EthernetSerdes.cpp.o: ../SilKit/source/services/ethernet/EthernetSerdes.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/EthernetSerdes.cpp.o"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/O_SilKit_Services_Ethernet.dir/EthernetSerdes.cpp.o -c /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/EthernetSerdes.cpp

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/EthernetSerdes.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/O_SilKit_Services_Ethernet.dir/EthernetSerdes.cpp.i"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/EthernetSerdes.cpp > CMakeFiles/O_SilKit_Services_Ethernet.dir/EthernetSerdes.cpp.i

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/EthernetSerdes.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/O_SilKit_Services_Ethernet.dir/EthernetSerdes.cpp.s"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet/EthernetSerdes.cpp -o CMakeFiles/O_SilKit_Services_Ethernet.dir/EthernetSerdes.cpp.s

O_SilKit_Services_Ethernet: SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/EthController.cpp.o
O_SilKit_Services_Ethernet: SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehavior.cpp.o
O_SilKit_Services_Ethernet: SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorDetailed.cpp.o
O_SilKit_Services_Ethernet: SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/SimBehaviorTrivial.cpp.o
O_SilKit_Services_Ethernet: SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/EthernetSerdes.cpp.o
O_SilKit_Services_Ethernet: SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/build.make

.PHONY : O_SilKit_Services_Ethernet

# Rule to build all files generated by this target.
SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/build: O_SilKit_Services_Ethernet

.PHONY : SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/build

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/clean:
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet && $(CMAKE_COMMAND) -P CMakeFiles/O_SilKit_Services_Ethernet.dir/cmake_clean.cmake
.PHONY : SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/clean

SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/depend:
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/source/services/ethernet /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : SilKit/source/services/ethernet/CMakeFiles/O_SilKit_Services_Ethernet.dir/depend

