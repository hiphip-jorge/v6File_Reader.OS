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
CMAKE_COMMAND = /cygdrive/c/Users/perez/AppData/Local/JetBrains/CLion2020.1/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/perez/AppData/Local/JetBrains/CLion2020.1/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Users/perez/CLionProjects/os4348-P2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Users/perez/CLionProjects/os4348-P2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/os4348-P2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/os4348-P2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/os4348-P2.dir/flags.make

CMakeFiles/os4348-P2.dir/main.c.o: CMakeFiles/os4348-P2.dir/flags.make
CMakeFiles/os4348-P2.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/perez/CLionProjects/os4348-P2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/os4348-P2.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/os4348-P2.dir/main.c.o   -c /cygdrive/c/Users/perez/CLionProjects/os4348-P2/main.c

CMakeFiles/os4348-P2.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/os4348-P2.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/Users/perez/CLionProjects/os4348-P2/main.c > CMakeFiles/os4348-P2.dir/main.c.i

CMakeFiles/os4348-P2.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/os4348-P2.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/Users/perez/CLionProjects/os4348-P2/main.c -o CMakeFiles/os4348-P2.dir/main.c.s

# Object files for target os4348-P2
os4348__P2_OBJECTS = \
"CMakeFiles/os4348-P2.dir/main.c.o"

# External object files for target os4348-P2
os4348__P2_EXTERNAL_OBJECTS =

os4348-P2.exe: CMakeFiles/os4348-P2.dir/main.c.o
os4348-P2.exe: CMakeFiles/os4348-P2.dir/build.make
os4348-P2.exe: CMakeFiles/os4348-P2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Users/perez/CLionProjects/os4348-P2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable os4348-P2.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/os4348-P2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/os4348-P2.dir/build: os4348-P2.exe

.PHONY : CMakeFiles/os4348-P2.dir/build

CMakeFiles/os4348-P2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/os4348-P2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/os4348-P2.dir/clean

CMakeFiles/os4348-P2.dir/depend:
	cd /cygdrive/c/Users/perez/CLionProjects/os4348-P2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Users/perez/CLionProjects/os4348-P2 /cygdrive/c/Users/perez/CLionProjects/os4348-P2 /cygdrive/c/Users/perez/CLionProjects/os4348-P2/cmake-build-debug /cygdrive/c/Users/perez/CLionProjects/os4348-P2/cmake-build-debug /cygdrive/c/Users/perez/CLionProjects/os4348-P2/cmake-build-debug/CMakeFiles/os4348-P2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/os4348-P2.dir/depend

