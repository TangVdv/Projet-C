# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/tangvdv/Documents/clion-2021.3/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/tangvdv/Documents/clion-2021.3/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tangvdv/CLionProjects/Projet-C

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tangvdv/CLionProjects/Projet-C/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Projet-C.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Projet-C.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Projet-C.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Projet-C.dir/flags.make

CMakeFiles/Projet-C.dir/main.c.o: CMakeFiles/Projet-C.dir/flags.make
CMakeFiles/Projet-C.dir/main.c.o: ../main.c
CMakeFiles/Projet-C.dir/main.c.o: CMakeFiles/Projet-C.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tangvdv/CLionProjects/Projet-C/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Projet-C.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Projet-C.dir/main.c.o -MF CMakeFiles/Projet-C.dir/main.c.o.d -o CMakeFiles/Projet-C.dir/main.c.o -c /home/tangvdv/CLionProjects/Projet-C/main.c

CMakeFiles/Projet-C.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Projet-C.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/tangvdv/CLionProjects/Projet-C/main.c > CMakeFiles/Projet-C.dir/main.c.i

CMakeFiles/Projet-C.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Projet-C.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/tangvdv/CLionProjects/Projet-C/main.c -o CMakeFiles/Projet-C.dir/main.c.s

# Object files for target Projet-C
Projet__C_OBJECTS = \
"CMakeFiles/Projet-C.dir/main.c.o"

# External object files for target Projet-C
Projet__C_EXTERNAL_OBJECTS =

Projet-C: CMakeFiles/Projet-C.dir/main.c.o
Projet-C: CMakeFiles/Projet-C.dir/build.make
Projet-C: CMakeFiles/Projet-C.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tangvdv/CLionProjects/Projet-C/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Projet-C"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Projet-C.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Projet-C.dir/build: Projet-C
.PHONY : CMakeFiles/Projet-C.dir/build

CMakeFiles/Projet-C.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Projet-C.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Projet-C.dir/clean

CMakeFiles/Projet-C.dir/depend:
	cd /home/tangvdv/CLionProjects/Projet-C/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tangvdv/CLionProjects/Projet-C /home/tangvdv/CLionProjects/Projet-C /home/tangvdv/CLionProjects/Projet-C/cmake-build-debug /home/tangvdv/CLionProjects/Projet-C/cmake-build-debug /home/tangvdv/CLionProjects/Projet-C/cmake-build-debug/CMakeFiles/Projet-C.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Projet-C.dir/depend

