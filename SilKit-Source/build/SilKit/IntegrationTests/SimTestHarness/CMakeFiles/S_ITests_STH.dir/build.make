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
include SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/depend.make

# Include the progress variables for this target.
include SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/progress.make

# Include the compile flags for this target's objects.
include SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/flags.make

SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/SimTestHarness.cpp.o: SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/flags.make
SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/SimTestHarness.cpp.o: ../SilKit/IntegrationTests/SimTestHarness/SimTestHarness.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/SimTestHarness.cpp.o"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/IntegrationTests/SimTestHarness && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/S_ITests_STH.dir/SimTestHarness.cpp.o -c /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/IntegrationTests/SimTestHarness/SimTestHarness.cpp

SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/SimTestHarness.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/S_ITests_STH.dir/SimTestHarness.cpp.i"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/IntegrationTests/SimTestHarness && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/IntegrationTests/SimTestHarness/SimTestHarness.cpp > CMakeFiles/S_ITests_STH.dir/SimTestHarness.cpp.i

SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/SimTestHarness.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/S_ITests_STH.dir/SimTestHarness.cpp.s"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/IntegrationTests/SimTestHarness && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/IntegrationTests/SimTestHarness/SimTestHarness.cpp -o CMakeFiles/S_ITests_STH.dir/SimTestHarness.cpp.s

# Object files for target S_ITests_STH
S_ITests_STH_OBJECTS = \
"CMakeFiles/S_ITests_STH.dir/SimTestHarness.cpp.o"

# External object files for target S_ITests_STH
S_ITests_STH_EXTERNAL_OBJECTS =

SilKit/IntegrationTests/SimTestHarness/libS_ITests_STH.a: SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/SimTestHarness.cpp.o
SilKit/IntegrationTests/SimTestHarness/libS_ITests_STH.a: SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/build.make
SilKit/IntegrationTests/SimTestHarness/libS_ITests_STH.a: SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libS_ITests_STH.a"
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/IntegrationTests/SimTestHarness && $(CMAKE_COMMAND) -P CMakeFiles/S_ITests_STH.dir/cmake_clean_target.cmake
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/IntegrationTests/SimTestHarness && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/S_ITests_STH.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/build: SilKit/IntegrationTests/SimTestHarness/libS_ITests_STH.a

.PHONY : SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/build

SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/clean:
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/IntegrationTests/SimTestHarness && $(CMAKE_COMMAND) -P CMakeFiles/S_ITests_STH.dir/cmake_clean.cmake
.PHONY : SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/clean

SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/depend:
	cd /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/SilKit/IntegrationTests/SimTestHarness /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/IntegrationTests/SimTestHarness /home/ubuntu2004/SilKit-4.0.50-ubuntu-18.04-x86_64-gcc/SilKit-Source/build/SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : SilKit/IntegrationTests/SimTestHarness/CMakeFiles/S_ITests_STH.dir/depend

