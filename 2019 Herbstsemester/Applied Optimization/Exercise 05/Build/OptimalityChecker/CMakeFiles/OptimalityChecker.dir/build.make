# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build"

# Include any dependencies generated for this target.
include OptimalityChecker/CMakeFiles/OptimalityChecker.dir/depend.make

# Include the progress variables for this target.
include OptimalityChecker/CMakeFiles/OptimalityChecker.dir/progress.make

# Include the compile flags for this target's objects.
include OptimalityChecker/CMakeFiles/OptimalityChecker.dir/flags.make

OptimalityChecker/CMakeFiles/OptimalityChecker.dir/main.cc.o: OptimalityChecker/CMakeFiles/OptimalityChecker.dir/flags.make
OptimalityChecker/CMakeFiles/OptimalityChecker.dir/main.cc.o: ../OptimalityChecker/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object OptimalityChecker/CMakeFiles/OptimalityChecker.dir/main.cc.o"
	cd "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/OptimalityChecker" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OptimalityChecker.dir/main.cc.o -c "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/OptimalityChecker/main.cc"

OptimalityChecker/CMakeFiles/OptimalityChecker.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OptimalityChecker.dir/main.cc.i"
	cd "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/OptimalityChecker" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/OptimalityChecker/main.cc" > CMakeFiles/OptimalityChecker.dir/main.cc.i

OptimalityChecker/CMakeFiles/OptimalityChecker.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OptimalityChecker.dir/main.cc.s"
	cd "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/OptimalityChecker" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/OptimalityChecker/main.cc" -o CMakeFiles/OptimalityChecker.dir/main.cc.s

OptimalityChecker/CMakeFiles/OptimalityChecker.dir/OptimalityChecker.cc.o: OptimalityChecker/CMakeFiles/OptimalityChecker.dir/flags.make
OptimalityChecker/CMakeFiles/OptimalityChecker.dir/OptimalityChecker.cc.o: ../OptimalityChecker/OptimalityChecker.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object OptimalityChecker/CMakeFiles/OptimalityChecker.dir/OptimalityChecker.cc.o"
	cd "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/OptimalityChecker" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OptimalityChecker.dir/OptimalityChecker.cc.o -c "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/OptimalityChecker/OptimalityChecker.cc"

OptimalityChecker/CMakeFiles/OptimalityChecker.dir/OptimalityChecker.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OptimalityChecker.dir/OptimalityChecker.cc.i"
	cd "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/OptimalityChecker" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/OptimalityChecker/OptimalityChecker.cc" > CMakeFiles/OptimalityChecker.dir/OptimalityChecker.cc.i

OptimalityChecker/CMakeFiles/OptimalityChecker.dir/OptimalityChecker.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OptimalityChecker.dir/OptimalityChecker.cc.s"
	cd "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/OptimalityChecker" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/OptimalityChecker/OptimalityChecker.cc" -o CMakeFiles/OptimalityChecker.dir/OptimalityChecker.cc.s

# Object files for target OptimalityChecker
OptimalityChecker_OBJECTS = \
"CMakeFiles/OptimalityChecker.dir/main.cc.o" \
"CMakeFiles/OptimalityChecker.dir/OptimalityChecker.cc.o"

# External object files for target OptimalityChecker
OptimalityChecker_EXTERNAL_OBJECTS =

Build/bin/OptimalityChecker: OptimalityChecker/CMakeFiles/OptimalityChecker.dir/main.cc.o
Build/bin/OptimalityChecker: OptimalityChecker/CMakeFiles/OptimalityChecker.dir/OptimalityChecker.cc.o
Build/bin/OptimalityChecker: OptimalityChecker/CMakeFiles/OptimalityChecker.dir/build.make
Build/bin/OptimalityChecker: OptimalityChecker/CMakeFiles/OptimalityChecker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../Build/bin/OptimalityChecker"
	cd "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/OptimalityChecker" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OptimalityChecker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
OptimalityChecker/CMakeFiles/OptimalityChecker.dir/build: Build/bin/OptimalityChecker

.PHONY : OptimalityChecker/CMakeFiles/OptimalityChecker.dir/build

OptimalityChecker/CMakeFiles/OptimalityChecker.dir/clean:
	cd "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/OptimalityChecker" && $(CMAKE_COMMAND) -P CMakeFiles/OptimalityChecker.dir/cmake_clean.cmake
.PHONY : OptimalityChecker/CMakeFiles/OptimalityChecker.dir/clean

OptimalityChecker/CMakeFiles/OptimalityChecker.dir/depend:
	cd "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05" "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/OptimalityChecker" "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build" "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/OptimalityChecker" "/home/lorenzo/Schreibtisch/Applied Optimization/AppliedOpti/Exercise 05/Build/OptimalityChecker/CMakeFiles/OptimalityChecker.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : OptimalityChecker/CMakeFiles/OptimalityChecker.dir/depend

