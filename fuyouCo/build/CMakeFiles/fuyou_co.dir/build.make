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
CMAKE_SOURCE_DIR = /home/hqin/fuyouCo/fuyouCo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hqin/fuyouCo/fuyouCo/build

# Include any dependencies generated for this target.
include CMakeFiles/fuyou_co.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/fuyou_co.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/fuyou_co.dir/flags.make

CMakeFiles/fuyou_co.dir/Coroutine.cpp.o: CMakeFiles/fuyou_co.dir/flags.make
CMakeFiles/fuyou_co.dir/Coroutine.cpp.o: ../Coroutine.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hqin/fuyouCo/fuyouCo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/fuyou_co.dir/Coroutine.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fuyou_co.dir/Coroutine.cpp.o -c /home/hqin/fuyouCo/fuyouCo/Coroutine.cpp

CMakeFiles/fuyou_co.dir/Coroutine.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fuyou_co.dir/Coroutine.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hqin/fuyouCo/fuyouCo/Coroutine.cpp > CMakeFiles/fuyou_co.dir/Coroutine.cpp.i

CMakeFiles/fuyou_co.dir/Coroutine.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fuyou_co.dir/Coroutine.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hqin/fuyouCo/fuyouCo/Coroutine.cpp -o CMakeFiles/fuyou_co.dir/Coroutine.cpp.s

CMakeFiles/fuyou_co.dir/Coroutine.cpp.o.requires:

.PHONY : CMakeFiles/fuyou_co.dir/Coroutine.cpp.o.requires

CMakeFiles/fuyou_co.dir/Coroutine.cpp.o.provides: CMakeFiles/fuyou_co.dir/Coroutine.cpp.o.requires
	$(MAKE) -f CMakeFiles/fuyou_co.dir/build.make CMakeFiles/fuyou_co.dir/Coroutine.cpp.o.provides.build
.PHONY : CMakeFiles/fuyou_co.dir/Coroutine.cpp.o.provides

CMakeFiles/fuyou_co.dir/Coroutine.cpp.o.provides.build: CMakeFiles/fuyou_co.dir/Coroutine.cpp.o


CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o: CMakeFiles/fuyou_co.dir/flags.make
CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o: ../CoScheduler.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hqin/fuyouCo/fuyouCo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o -c /home/hqin/fuyouCo/fuyouCo/CoScheduler.cpp

CMakeFiles/fuyou_co.dir/CoScheduler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fuyou_co.dir/CoScheduler.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hqin/fuyouCo/fuyouCo/CoScheduler.cpp > CMakeFiles/fuyou_co.dir/CoScheduler.cpp.i

CMakeFiles/fuyou_co.dir/CoScheduler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fuyou_co.dir/CoScheduler.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hqin/fuyouCo/fuyouCo/CoScheduler.cpp -o CMakeFiles/fuyou_co.dir/CoScheduler.cpp.s

CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o.requires:

.PHONY : CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o.requires

CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o.provides: CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o.requires
	$(MAKE) -f CMakeFiles/fuyou_co.dir/build.make CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o.provides.build
.PHONY : CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o.provides

CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o.provides.build: CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o


CMakeFiles/fuyou_co.dir/Epoll.cpp.o: CMakeFiles/fuyou_co.dir/flags.make
CMakeFiles/fuyou_co.dir/Epoll.cpp.o: ../Epoll.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hqin/fuyouCo/fuyouCo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/fuyou_co.dir/Epoll.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fuyou_co.dir/Epoll.cpp.o -c /home/hqin/fuyouCo/fuyouCo/Epoll.cpp

CMakeFiles/fuyou_co.dir/Epoll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fuyou_co.dir/Epoll.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hqin/fuyouCo/fuyouCo/Epoll.cpp > CMakeFiles/fuyou_co.dir/Epoll.cpp.i

CMakeFiles/fuyou_co.dir/Epoll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fuyou_co.dir/Epoll.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hqin/fuyouCo/fuyouCo/Epoll.cpp -o CMakeFiles/fuyou_co.dir/Epoll.cpp.s

CMakeFiles/fuyou_co.dir/Epoll.cpp.o.requires:

.PHONY : CMakeFiles/fuyou_co.dir/Epoll.cpp.o.requires

CMakeFiles/fuyou_co.dir/Epoll.cpp.o.provides: CMakeFiles/fuyou_co.dir/Epoll.cpp.o.requires
	$(MAKE) -f CMakeFiles/fuyou_co.dir/build.make CMakeFiles/fuyou_co.dir/Epoll.cpp.o.provides.build
.PHONY : CMakeFiles/fuyou_co.dir/Epoll.cpp.o.provides

CMakeFiles/fuyou_co.dir/Epoll.cpp.o.provides.build: CMakeFiles/fuyou_co.dir/Epoll.cpp.o


CMakeFiles/fuyou_co.dir/Socket.cpp.o: CMakeFiles/fuyou_co.dir/flags.make
CMakeFiles/fuyou_co.dir/Socket.cpp.o: ../Socket.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hqin/fuyouCo/fuyouCo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/fuyou_co.dir/Socket.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fuyou_co.dir/Socket.cpp.o -c /home/hqin/fuyouCo/fuyouCo/Socket.cpp

CMakeFiles/fuyou_co.dir/Socket.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fuyou_co.dir/Socket.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hqin/fuyouCo/fuyouCo/Socket.cpp > CMakeFiles/fuyou_co.dir/Socket.cpp.i

CMakeFiles/fuyou_co.dir/Socket.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fuyou_co.dir/Socket.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hqin/fuyouCo/fuyouCo/Socket.cpp -o CMakeFiles/fuyou_co.dir/Socket.cpp.s

CMakeFiles/fuyou_co.dir/Socket.cpp.o.requires:

.PHONY : CMakeFiles/fuyou_co.dir/Socket.cpp.o.requires

CMakeFiles/fuyou_co.dir/Socket.cpp.o.provides: CMakeFiles/fuyou_co.dir/Socket.cpp.o.requires
	$(MAKE) -f CMakeFiles/fuyou_co.dir/build.make CMakeFiles/fuyou_co.dir/Socket.cpp.o.provides.build
.PHONY : CMakeFiles/fuyou_co.dir/Socket.cpp.o.provides

CMakeFiles/fuyou_co.dir/Socket.cpp.o.provides.build: CMakeFiles/fuyou_co.dir/Socket.cpp.o


# Object files for target fuyou_co
fuyou_co_OBJECTS = \
"CMakeFiles/fuyou_co.dir/Coroutine.cpp.o" \
"CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o" \
"CMakeFiles/fuyou_co.dir/Epoll.cpp.o" \
"CMakeFiles/fuyou_co.dir/Socket.cpp.o"

# External object files for target fuyou_co
fuyou_co_EXTERNAL_OBJECTS =

libfuyou_co.a: CMakeFiles/fuyou_co.dir/Coroutine.cpp.o
libfuyou_co.a: CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o
libfuyou_co.a: CMakeFiles/fuyou_co.dir/Epoll.cpp.o
libfuyou_co.a: CMakeFiles/fuyou_co.dir/Socket.cpp.o
libfuyou_co.a: CMakeFiles/fuyou_co.dir/build.make
libfuyou_co.a: CMakeFiles/fuyou_co.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hqin/fuyouCo/fuyouCo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library libfuyou_co.a"
	$(CMAKE_COMMAND) -P CMakeFiles/fuyou_co.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fuyou_co.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/fuyou_co.dir/build: libfuyou_co.a

.PHONY : CMakeFiles/fuyou_co.dir/build

CMakeFiles/fuyou_co.dir/requires: CMakeFiles/fuyou_co.dir/Coroutine.cpp.o.requires
CMakeFiles/fuyou_co.dir/requires: CMakeFiles/fuyou_co.dir/CoScheduler.cpp.o.requires
CMakeFiles/fuyou_co.dir/requires: CMakeFiles/fuyou_co.dir/Epoll.cpp.o.requires
CMakeFiles/fuyou_co.dir/requires: CMakeFiles/fuyou_co.dir/Socket.cpp.o.requires

.PHONY : CMakeFiles/fuyou_co.dir/requires

CMakeFiles/fuyou_co.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/fuyou_co.dir/cmake_clean.cmake
.PHONY : CMakeFiles/fuyou_co.dir/clean

CMakeFiles/fuyou_co.dir/depend:
	cd /home/hqin/fuyouCo/fuyouCo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hqin/fuyouCo/fuyouCo /home/hqin/fuyouCo/fuyouCo /home/hqin/fuyouCo/fuyouCo/build /home/hqin/fuyouCo/fuyouCo/build /home/hqin/fuyouCo/fuyouCo/build/CMakeFiles/fuyou_co.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/fuyou_co.dir/depend
