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
CMAKE_SOURCE_DIR = /home/pi/Desktop/DEFFS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/Desktop/DEFFS

# Include any dependencies generated for this target.
include CMakeFiles/DEFFS.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DEFFS.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DEFFS.dir/flags.make

CMakeFiles/DEFFS.dir/src/deffs.c.o: CMakeFiles/DEFFS.dir/flags.make
CMakeFiles/DEFFS.dir/src/deffs.c.o: src/deffs.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/DEFFS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/DEFFS.dir/src/deffs.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DEFFS.dir/src/deffs.c.o   -c /home/pi/Desktop/DEFFS/src/deffs.c

CMakeFiles/DEFFS.dir/src/deffs.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DEFFS.dir/src/deffs.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Desktop/DEFFS/src/deffs.c > CMakeFiles/DEFFS.dir/src/deffs.c.i

CMakeFiles/DEFFS.dir/src/deffs.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DEFFS.dir/src/deffs.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Desktop/DEFFS/src/deffs.c -o CMakeFiles/DEFFS.dir/src/deffs.c.s

CMakeFiles/DEFFS.dir/src/attr.c.o: CMakeFiles/DEFFS.dir/flags.make
CMakeFiles/DEFFS.dir/src/attr.c.o: src/attr.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/DEFFS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/DEFFS.dir/src/attr.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DEFFS.dir/src/attr.c.o   -c /home/pi/Desktop/DEFFS/src/attr.c

CMakeFiles/DEFFS.dir/src/attr.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DEFFS.dir/src/attr.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Desktop/DEFFS/src/attr.c > CMakeFiles/DEFFS.dir/src/attr.c.i

CMakeFiles/DEFFS.dir/src/attr.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DEFFS.dir/src/attr.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Desktop/DEFFS/src/attr.c -o CMakeFiles/DEFFS.dir/src/attr.c.s

CMakeFiles/DEFFS.dir/src/utils.c.o: CMakeFiles/DEFFS.dir/flags.make
CMakeFiles/DEFFS.dir/src/utils.c.o: src/utils.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/DEFFS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/DEFFS.dir/src/utils.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DEFFS.dir/src/utils.c.o   -c /home/pi/Desktop/DEFFS/src/utils.c

CMakeFiles/DEFFS.dir/src/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DEFFS.dir/src/utils.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Desktop/DEFFS/src/utils.c > CMakeFiles/DEFFS.dir/src/utils.c.i

CMakeFiles/DEFFS.dir/src/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DEFFS.dir/src/utils.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Desktop/DEFFS/src/utils.c -o CMakeFiles/DEFFS.dir/src/utils.c.s

CMakeFiles/DEFFS.dir/src/arguments.c.o: CMakeFiles/DEFFS.dir/flags.make
CMakeFiles/DEFFS.dir/src/arguments.c.o: src/arguments.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/DEFFS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/DEFFS.dir/src/arguments.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DEFFS.dir/src/arguments.c.o   -c /home/pi/Desktop/DEFFS/src/arguments.c

CMakeFiles/DEFFS.dir/src/arguments.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DEFFS.dir/src/arguments.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Desktop/DEFFS/src/arguments.c > CMakeFiles/DEFFS.dir/src/arguments.c.i

CMakeFiles/DEFFS.dir/src/arguments.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DEFFS.dir/src/arguments.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Desktop/DEFFS/src/arguments.c -o CMakeFiles/DEFFS.dir/src/arguments.c.s

CMakeFiles/DEFFS.dir/src/rw.c.o: CMakeFiles/DEFFS.dir/flags.make
CMakeFiles/DEFFS.dir/src/rw.c.o: src/rw.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/DEFFS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/DEFFS.dir/src/rw.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DEFFS.dir/src/rw.c.o   -c /home/pi/Desktop/DEFFS/src/rw.c

CMakeFiles/DEFFS.dir/src/rw.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DEFFS.dir/src/rw.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Desktop/DEFFS/src/rw.c > CMakeFiles/DEFFS.dir/src/rw.c.i

CMakeFiles/DEFFS.dir/src/rw.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DEFFS.dir/src/rw.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Desktop/DEFFS/src/rw.c -o CMakeFiles/DEFFS.dir/src/rw.c.s

CMakeFiles/DEFFS.dir/src/crypto.c.o: CMakeFiles/DEFFS.dir/flags.make
CMakeFiles/DEFFS.dir/src/crypto.c.o: src/crypto.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/DEFFS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/DEFFS.dir/src/crypto.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DEFFS.dir/src/crypto.c.o   -c /home/pi/Desktop/DEFFS/src/crypto.c

CMakeFiles/DEFFS.dir/src/crypto.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DEFFS.dir/src/crypto.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Desktop/DEFFS/src/crypto.c > CMakeFiles/DEFFS.dir/src/crypto.c.i

CMakeFiles/DEFFS.dir/src/crypto.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DEFFS.dir/src/crypto.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Desktop/DEFFS/src/crypto.c -o CMakeFiles/DEFFS.dir/src/crypto.c.s

CMakeFiles/DEFFS.dir/src/perms.c.o: CMakeFiles/DEFFS.dir/flags.make
CMakeFiles/DEFFS.dir/src/perms.c.o: src/perms.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/Desktop/DEFFS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/DEFFS.dir/src/perms.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/DEFFS.dir/src/perms.c.o   -c /home/pi/Desktop/DEFFS/src/perms.c

CMakeFiles/DEFFS.dir/src/perms.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DEFFS.dir/src/perms.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/Desktop/DEFFS/src/perms.c > CMakeFiles/DEFFS.dir/src/perms.c.i

CMakeFiles/DEFFS.dir/src/perms.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DEFFS.dir/src/perms.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/Desktop/DEFFS/src/perms.c -o CMakeFiles/DEFFS.dir/src/perms.c.s

# Object files for target DEFFS
DEFFS_OBJECTS = \
"CMakeFiles/DEFFS.dir/src/deffs.c.o" \
"CMakeFiles/DEFFS.dir/src/attr.c.o" \
"CMakeFiles/DEFFS.dir/src/utils.c.o" \
"CMakeFiles/DEFFS.dir/src/arguments.c.o" \
"CMakeFiles/DEFFS.dir/src/rw.c.o" \
"CMakeFiles/DEFFS.dir/src/crypto.c.o" \
"CMakeFiles/DEFFS.dir/src/perms.c.o"

# External object files for target DEFFS
DEFFS_EXTERNAL_OBJECTS =

bin/DEFFS: CMakeFiles/DEFFS.dir/src/deffs.c.o
bin/DEFFS: CMakeFiles/DEFFS.dir/src/attr.c.o
bin/DEFFS: CMakeFiles/DEFFS.dir/src/utils.c.o
bin/DEFFS: CMakeFiles/DEFFS.dir/src/arguments.c.o
bin/DEFFS: CMakeFiles/DEFFS.dir/src/rw.c.o
bin/DEFFS: CMakeFiles/DEFFS.dir/src/crypto.c.o
bin/DEFFS: CMakeFiles/DEFFS.dir/src/perms.c.o
bin/DEFFS: CMakeFiles/DEFFS.dir/build.make
bin/DEFFS: /usr/lib/arm-linux-gnueabihf/libfuse.so
bin/DEFFS: CMakeFiles/DEFFS.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/Desktop/DEFFS/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking C executable bin/DEFFS"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DEFFS.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DEFFS.dir/build: bin/DEFFS

.PHONY : CMakeFiles/DEFFS.dir/build

CMakeFiles/DEFFS.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DEFFS.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DEFFS.dir/clean

CMakeFiles/DEFFS.dir/depend:
	cd /home/pi/Desktop/DEFFS && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/Desktop/DEFFS /home/pi/Desktop/DEFFS /home/pi/Desktop/DEFFS /home/pi/Desktop/DEFFS /home/pi/Desktop/DEFFS/CMakeFiles/DEFFS.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DEFFS.dir/depend
