# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /home/charlesaverill/anaconda3/bin/cmake

# The command to remove a file.
RM = /home/charlesaverill/anaconda3/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/charlesaverill/CProjects/DEFFS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/charlesaverill/CProjects/DEFFS

# Include any dependencies generated for this target.
include CMakeFiles/DEFFS.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DEFFS.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DEFFS.dir/flags.make

CMakeFiles/DEFFS.dir/src/main.c.o: CMakeFiles/DEFFS.dir/flags.make
CMakeFiles/DEFFS.dir/src/main.c.o: src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/charlesaverill/CProjects/DEFFS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/DEFFS.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DEFFS.dir/src/main.c.o   -c /home/charlesaverill/CProjects/DEFFS/src/main.c

CMakeFiles/DEFFS.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DEFFS.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/charlesaverill/CProjects/DEFFS/src/main.c > CMakeFiles/DEFFS.dir/src/main.c.i

CMakeFiles/DEFFS.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DEFFS.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/charlesaverill/CProjects/DEFFS/src/main.c -o CMakeFiles/DEFFS.dir/src/main.c.s

# Object files for target DEFFS
DEFFS_OBJECTS = \
"CMakeFiles/DEFFS.dir/src/main.c.o"

# External object files for target DEFFS
DEFFS_EXTERNAL_OBJECTS =

builds/DEFFS: CMakeFiles/DEFFS.dir/src/main.c.o
builds/DEFFS: CMakeFiles/DEFFS.dir/build.make
builds/DEFFS: /usr/lib/x86_64-linux-gnu/libfuse.so
builds/DEFFS: CMakeFiles/DEFFS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/charlesaverill/CProjects/DEFFS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable builds/DEFFS"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DEFFS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DEFFS.dir/build: builds/DEFFS

.PHONY : CMakeFiles/DEFFS.dir/build

CMakeFiles/DEFFS.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DEFFS.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DEFFS.dir/clean

CMakeFiles/DEFFS.dir/depend:
	cd /home/charlesaverill/CProjects/DEFFS && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/charlesaverill/CProjects/DEFFS /home/charlesaverill/CProjects/DEFFS /home/charlesaverill/CProjects/DEFFS /home/charlesaverill/CProjects/DEFFS /home/charlesaverill/CProjects/DEFFS/CMakeFiles/DEFFS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DEFFS.dir/depend

