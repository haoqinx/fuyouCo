# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/hqin/fuyouCo/fuyouCo/samples

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hqin/fuyouCo/fuyouCo/samples/build

# Include any dependencies generated for this target.
include CMakeFiles/server_base.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server_base.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server_base.dir/flags.make

CMakeFiles/server_base.dir/serverBase.cpp.o: CMakeFiles/server_base.dir/flags.make
CMakeFiles/server_base.dir/serverBase.cpp.o: ../serverBase.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hqin/fuyouCo/fuyouCo/samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/server_base.dir/serverBase.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server_base.dir/serverBase.cpp.o -c /home/hqin/fuyouCo/fuyouCo/samples/serverBase.cpp

CMakeFiles/server_base.dir/serverBase.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server_base.dir/serverBase.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hqin/fuyouCo/fuyouCo/samples/serverBase.cpp > CMakeFiles/server_base.dir/serverBase.cpp.i

CMakeFiles/server_base.dir/serverBase.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server_base.dir/serverBase.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hqin/fuyouCo/fuyouCo/samples/serverBase.cpp -o CMakeFiles/server_base.dir/serverBase.cpp.s

CMakeFiles/server_base.dir/serverBase.cpp.o.requires:

.PHONY : CMakeFiles/server_base.dir/serverBase.cpp.o.requires

CMakeFiles/server_base.dir/serverBase.cpp.o.provides: CMakeFiles/server_base.dir/serverBase.cpp.o.requires
	$(MAKE) -f CMakeFiles/server_base.dir/build.make CMakeFiles/server_base.dir/serverBase.cpp.o.provides.build
.PHONY : CMakeFiles/server_base.dir/serverBase.cpp.o.provides

CMakeFiles/server_base.dir/serverBase.cpp.o.provides.build: CMakeFiles/server_base.dir/serverBase.cpp.o


# Object files for target server_base
server_base_OBJECTS = \
"CMakeFiles/server_base.dir/serverBase.cpp.o"

# External object files for target server_base
server_base_EXTERNAL_OBJECTS =

server_base: CMakeFiles/server_base.dir/serverBase.cpp.o
server_base: CMakeFiles/server_base.dir/build.make
server_base: CMakeFiles/server_base.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hqin/fuyouCo/fuyouCo/samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable server_base"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server_base.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server_base.dir/build: server_base

.PHONY : CMakeFiles/server_base.dir/build

CMakeFiles/server_base.dir/requires: CMakeFiles/server_base.dir/serverBase.cpp.o.requires

.PHONY : CMakeFiles/server_base.dir/requires

CMakeFiles/server_base.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server_base.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server_base.dir/clean

CMakeFiles/server_base.dir/depend:
	cd /home/hqin/fuyouCo/fuyouCo/samples/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hqin/fuyouCo/fuyouCo/samples /home/hqin/fuyouCo/fuyouCo/samples /home/hqin/fuyouCo/fuyouCo/samples/build /home/hqin/fuyouCo/fuyouCo/samples/build /home/hqin/fuyouCo/fuyouCo/samples/build/CMakeFiles/server_base.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server_base.dir/depend
