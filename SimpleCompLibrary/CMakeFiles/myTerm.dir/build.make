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
CMAKE_SOURCE_DIR = /mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary

# Include any dependencies generated for this target.
include CMakeFiles/myTerm.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/myTerm.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myTerm.dir/flags.make

CMakeFiles/myTerm.dir/terminal.c.o: CMakeFiles/myTerm.dir/flags.make
CMakeFiles/myTerm.dir/terminal.c.o: terminal.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/myTerm.dir/terminal.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/myTerm.dir/terminal.c.o   -c /mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary/terminal.c

CMakeFiles/myTerm.dir/terminal.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/myTerm.dir/terminal.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary/terminal.c > CMakeFiles/myTerm.dir/terminal.c.i

CMakeFiles/myTerm.dir/terminal.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/myTerm.dir/terminal.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary/terminal.c -o CMakeFiles/myTerm.dir/terminal.c.s

# Object files for target myTerm
myTerm_OBJECTS = \
"CMakeFiles/myTerm.dir/terminal.c.o"

# External object files for target myTerm
myTerm_EXTERNAL_OBJECTS =

libmyTerm.a: CMakeFiles/myTerm.dir/terminal.c.o
libmyTerm.a: CMakeFiles/myTerm.dir/build.make
libmyTerm.a: CMakeFiles/myTerm.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libmyTerm.a"
	$(CMAKE_COMMAND) -P CMakeFiles/myTerm.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myTerm.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myTerm.dir/build: libmyTerm.a

.PHONY : CMakeFiles/myTerm.dir/build

CMakeFiles/myTerm.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myTerm.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myTerm.dir/clean

CMakeFiles/myTerm.dir/depend:
	cd /mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary /mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary /mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary /mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary /mnt/c/Users/frutl/Desktop/VYZ/prog/Architecture/src/SimpleCompLibrary/CMakeFiles/myTerm.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/myTerm.dir/depend
