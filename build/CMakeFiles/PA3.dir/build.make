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
CMAKE_SOURCE_DIR = /home/yikai/Documents/computer-graphics-project-2021-spring

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yikai/Documents/computer-graphics-project-2021-spring/build

# Include any dependencies generated for this target.
include CMakeFiles/PA3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/PA3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PA3.dir/flags.make

CMakeFiles/PA3.dir/src/image.cpp.o: CMakeFiles/PA3.dir/flags.make
CMakeFiles/PA3.dir/src/image.cpp.o: ../src/image.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yikai/Documents/computer-graphics-project-2021-spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/PA3.dir/src/image.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PA3.dir/src/image.cpp.o -c /home/yikai/Documents/computer-graphics-project-2021-spring/src/image.cpp

CMakeFiles/PA3.dir/src/image.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PA3.dir/src/image.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yikai/Documents/computer-graphics-project-2021-spring/src/image.cpp > CMakeFiles/PA3.dir/src/image.cpp.i

CMakeFiles/PA3.dir/src/image.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PA3.dir/src/image.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yikai/Documents/computer-graphics-project-2021-spring/src/image.cpp -o CMakeFiles/PA3.dir/src/image.cpp.s

CMakeFiles/PA3.dir/src/main.cpp.o: CMakeFiles/PA3.dir/flags.make
CMakeFiles/PA3.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yikai/Documents/computer-graphics-project-2021-spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/PA3.dir/src/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PA3.dir/src/main.cpp.o -c /home/yikai/Documents/computer-graphics-project-2021-spring/src/main.cpp

CMakeFiles/PA3.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PA3.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yikai/Documents/computer-graphics-project-2021-spring/src/main.cpp > CMakeFiles/PA3.dir/src/main.cpp.i

CMakeFiles/PA3.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PA3.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yikai/Documents/computer-graphics-project-2021-spring/src/main.cpp -o CMakeFiles/PA3.dir/src/main.cpp.s

CMakeFiles/PA3.dir/src/mesh.cpp.o: CMakeFiles/PA3.dir/flags.make
CMakeFiles/PA3.dir/src/mesh.cpp.o: ../src/mesh.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yikai/Documents/computer-graphics-project-2021-spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/PA3.dir/src/mesh.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PA3.dir/src/mesh.cpp.o -c /home/yikai/Documents/computer-graphics-project-2021-spring/src/mesh.cpp

CMakeFiles/PA3.dir/src/mesh.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PA3.dir/src/mesh.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yikai/Documents/computer-graphics-project-2021-spring/src/mesh.cpp > CMakeFiles/PA3.dir/src/mesh.cpp.i

CMakeFiles/PA3.dir/src/mesh.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PA3.dir/src/mesh.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yikai/Documents/computer-graphics-project-2021-spring/src/mesh.cpp -o CMakeFiles/PA3.dir/src/mesh.cpp.s

CMakeFiles/PA3.dir/src/scene_parser.cpp.o: CMakeFiles/PA3.dir/flags.make
CMakeFiles/PA3.dir/src/scene_parser.cpp.o: ../src/scene_parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yikai/Documents/computer-graphics-project-2021-spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/PA3.dir/src/scene_parser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PA3.dir/src/scene_parser.cpp.o -c /home/yikai/Documents/computer-graphics-project-2021-spring/src/scene_parser.cpp

CMakeFiles/PA3.dir/src/scene_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PA3.dir/src/scene_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yikai/Documents/computer-graphics-project-2021-spring/src/scene_parser.cpp > CMakeFiles/PA3.dir/src/scene_parser.cpp.i

CMakeFiles/PA3.dir/src/scene_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PA3.dir/src/scene_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yikai/Documents/computer-graphics-project-2021-spring/src/scene_parser.cpp -o CMakeFiles/PA3.dir/src/scene_parser.cpp.s

CMakeFiles/PA3.dir/src/camera_controller.cpp.o: CMakeFiles/PA3.dir/flags.make
CMakeFiles/PA3.dir/src/camera_controller.cpp.o: ../src/camera_controller.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yikai/Documents/computer-graphics-project-2021-spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/PA3.dir/src/camera_controller.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PA3.dir/src/camera_controller.cpp.o -c /home/yikai/Documents/computer-graphics-project-2021-spring/src/camera_controller.cpp

CMakeFiles/PA3.dir/src/camera_controller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PA3.dir/src/camera_controller.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yikai/Documents/computer-graphics-project-2021-spring/src/camera_controller.cpp > CMakeFiles/PA3.dir/src/camera_controller.cpp.i

CMakeFiles/PA3.dir/src/camera_controller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PA3.dir/src/camera_controller.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yikai/Documents/computer-graphics-project-2021-spring/src/camera_controller.cpp -o CMakeFiles/PA3.dir/src/camera_controller.cpp.s

# Object files for target PA3
PA3_OBJECTS = \
"CMakeFiles/PA3.dir/src/image.cpp.o" \
"CMakeFiles/PA3.dir/src/main.cpp.o" \
"CMakeFiles/PA3.dir/src/mesh.cpp.o" \
"CMakeFiles/PA3.dir/src/scene_parser.cpp.o" \
"CMakeFiles/PA3.dir/src/camera_controller.cpp.o"

# External object files for target PA3
PA3_EXTERNAL_OBJECTS =

../bin/PA3: CMakeFiles/PA3.dir/src/image.cpp.o
../bin/PA3: CMakeFiles/PA3.dir/src/main.cpp.o
../bin/PA3: CMakeFiles/PA3.dir/src/mesh.cpp.o
../bin/PA3: CMakeFiles/PA3.dir/src/scene_parser.cpp.o
../bin/PA3: CMakeFiles/PA3.dir/src/camera_controller.cpp.o
../bin/PA3: CMakeFiles/PA3.dir/build.make
../bin/PA3: deps/vecmath/libvecmath.a
../bin/PA3: /usr/lib/x86_64-linux-gnu/libGL.so
../bin/PA3: /usr/lib/x86_64-linux-gnu/libGLU.so
../bin/PA3: /usr/lib/x86_64-linux-gnu/libglut.so
../bin/PA3: CMakeFiles/PA3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yikai/Documents/computer-graphics-project-2021-spring/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable ../bin/PA3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PA3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PA3.dir/build: ../bin/PA3

.PHONY : CMakeFiles/PA3.dir/build

CMakeFiles/PA3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PA3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PA3.dir/clean

CMakeFiles/PA3.dir/depend:
	cd /home/yikai/Documents/computer-graphics-project-2021-spring/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yikai/Documents/computer-graphics-project-2021-spring /home/yikai/Documents/computer-graphics-project-2021-spring /home/yikai/Documents/computer-graphics-project-2021-spring/build /home/yikai/Documents/computer-graphics-project-2021-spring/build /home/yikai/Documents/computer-graphics-project-2021-spring/build/CMakeFiles/PA3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PA3.dir/depend
