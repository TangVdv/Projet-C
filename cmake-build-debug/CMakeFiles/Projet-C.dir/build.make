# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\tangr\CLionProjects\git_Projet-C

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\tangr\CLionProjects\git_Projet-C\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Projet-C.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/Projet-C.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Projet-C.dir/flags.make

CMakeFiles/Projet-C.dir/main.c.obj: CMakeFiles/Projet-C.dir/flags.make
CMakeFiles/Projet-C.dir/main.c.obj: CMakeFiles/Projet-C.dir/includes_C.rsp
CMakeFiles/Projet-C.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\tangr\CLionProjects\git_Projet-C\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Projet-C.dir/main.c.obj"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\Projet-C.dir\main.c.obj -c C:\Users\tangr\CLionProjects\git_Projet-C\main.c

CMakeFiles/Projet-C.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Projet-C.dir/main.c.i"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\tangr\CLionProjects\git_Projet-C\main.c > CMakeFiles\Projet-C.dir\main.c.i

CMakeFiles/Projet-C.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Projet-C.dir/main.c.s"
	C:\msys64\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\tangr\CLionProjects\git_Projet-C\main.c -o CMakeFiles\Projet-C.dir\main.c.s

# Object files for target Projet-C
Projet__C_OBJECTS = \
"CMakeFiles/Projet-C.dir/main.c.obj"

# External object files for target Projet-C
Projet__C_EXTERNAL_OBJECTS =

Projet-C.exe: CMakeFiles/Projet-C.dir/main.c.obj
Projet-C.exe: CMakeFiles/Projet-C.dir/build.make
Projet-C.exe: CMakeFiles/Projet-C.dir/linklibs.rsp
Projet-C.exe: CMakeFiles/Projet-C.dir/objects1.rsp
Projet-C.exe: CMakeFiles/Projet-C.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\tangr\CLionProjects\git_Projet-C\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Projet-C.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Projet-C.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Projet-C.dir/build: Projet-C.exe
.PHONY : CMakeFiles/Projet-C.dir/build

CMakeFiles/Projet-C.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Projet-C.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Projet-C.dir/clean

CMakeFiles/Projet-C.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\tangr\CLionProjects\git_Projet-C C:\Users\tangr\CLionProjects\git_Projet-C C:\Users\tangr\CLionProjects\git_Projet-C\cmake-build-debug C:\Users\tangr\CLionProjects\git_Projet-C\cmake-build-debug C:\Users\tangr\CLionProjects\git_Projet-C\cmake-build-debug\CMakeFiles\Projet-C.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Projet-C.dir/depend

