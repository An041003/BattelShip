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
CMAKE_SOURCE_DIR = /home/an/Documents/GitHub/BattelShip

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/an/Documents/GitHub/BattelShip/build

# Include any dependencies generated for this target.
include CMakeFiles/Server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Server.dir/flags.make

CMakeFiles/Server.dir/server.c.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/server.c.o: ../server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/an/Documents/GitHub/BattelShip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Server.dir/server.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Server.dir/server.c.o   -c /home/an/Documents/GitHub/BattelShip/server.c

CMakeFiles/Server.dir/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server.dir/server.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/an/Documents/GitHub/BattelShip/server.c > CMakeFiles/Server.dir/server.c.i

CMakeFiles/Server.dir/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server.dir/server.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/an/Documents/GitHub/BattelShip/server.c -o CMakeFiles/Server.dir/server.c.s

CMakeFiles/Server.dir/model/auth.c.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/model/auth.c.o: ../model/auth.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/an/Documents/GitHub/BattelShip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Server.dir/model/auth.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Server.dir/model/auth.c.o   -c /home/an/Documents/GitHub/BattelShip/model/auth.c

CMakeFiles/Server.dir/model/auth.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server.dir/model/auth.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/an/Documents/GitHub/BattelShip/model/auth.c > CMakeFiles/Server.dir/model/auth.c.i

CMakeFiles/Server.dir/model/auth.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server.dir/model/auth.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/an/Documents/GitHub/BattelShip/model/auth.c -o CMakeFiles/Server.dir/model/auth.c.s

CMakeFiles/Server.dir/model/network.c.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/model/network.c.o: ../model/network.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/an/Documents/GitHub/BattelShip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Server.dir/model/network.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Server.dir/model/network.c.o   -c /home/an/Documents/GitHub/BattelShip/model/network.c

CMakeFiles/Server.dir/model/network.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server.dir/model/network.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/an/Documents/GitHub/BattelShip/model/network.c > CMakeFiles/Server.dir/model/network.c.i

CMakeFiles/Server.dir/model/network.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server.dir/model/network.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/an/Documents/GitHub/BattelShip/model/network.c -o CMakeFiles/Server.dir/model/network.c.s

CMakeFiles/Server.dir/view/base_view.c.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/view/base_view.c.o: ../view/base_view.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/an/Documents/GitHub/BattelShip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/Server.dir/view/base_view.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Server.dir/view/base_view.c.o   -c /home/an/Documents/GitHub/BattelShip/view/base_view.c

CMakeFiles/Server.dir/view/base_view.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server.dir/view/base_view.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/an/Documents/GitHub/BattelShip/view/base_view.c > CMakeFiles/Server.dir/view/base_view.c.i

CMakeFiles/Server.dir/view/base_view.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server.dir/view/base_view.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/an/Documents/GitHub/BattelShip/view/base_view.c -o CMakeFiles/Server.dir/view/base_view.c.s

CMakeFiles/Server.dir/view/login_view.c.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/view/login_view.c.o: ../view/login_view.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/an/Documents/GitHub/BattelShip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/Server.dir/view/login_view.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Server.dir/view/login_view.c.o   -c /home/an/Documents/GitHub/BattelShip/view/login_view.c

CMakeFiles/Server.dir/view/login_view.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server.dir/view/login_view.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/an/Documents/GitHub/BattelShip/view/login_view.c > CMakeFiles/Server.dir/view/login_view.c.i

CMakeFiles/Server.dir/view/login_view.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server.dir/view/login_view.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/an/Documents/GitHub/BattelShip/view/login_view.c -o CMakeFiles/Server.dir/view/login_view.c.s

CMakeFiles/Server.dir/view/register_view.c.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/view/register_view.c.o: ../view/register_view.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/an/Documents/GitHub/BattelShip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/Server.dir/view/register_view.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Server.dir/view/register_view.c.o   -c /home/an/Documents/GitHub/BattelShip/view/register_view.c

CMakeFiles/Server.dir/view/register_view.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server.dir/view/register_view.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/an/Documents/GitHub/BattelShip/view/register_view.c > CMakeFiles/Server.dir/view/register_view.c.i

CMakeFiles/Server.dir/view/register_view.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server.dir/view/register_view.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/an/Documents/GitHub/BattelShip/view/register_view.c -o CMakeFiles/Server.dir/view/register_view.c.s

CMakeFiles/Server.dir/view/menu_view.c.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/view/menu_view.c.o: ../view/menu_view.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/an/Documents/GitHub/BattelShip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/Server.dir/view/menu_view.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Server.dir/view/menu_view.c.o   -c /home/an/Documents/GitHub/BattelShip/view/menu_view.c

CMakeFiles/Server.dir/view/menu_view.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server.dir/view/menu_view.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/an/Documents/GitHub/BattelShip/view/menu_view.c > CMakeFiles/Server.dir/view/menu_view.c.i

CMakeFiles/Server.dir/view/menu_view.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server.dir/view/menu_view.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/an/Documents/GitHub/BattelShip/view/menu_view.c -o CMakeFiles/Server.dir/view/menu_view.c.s

CMakeFiles/Server.dir/controller/register.c.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/controller/register.c.o: ../controller/register.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/an/Documents/GitHub/BattelShip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/Server.dir/controller/register.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Server.dir/controller/register.c.o   -c /home/an/Documents/GitHub/BattelShip/controller/register.c

CMakeFiles/Server.dir/controller/register.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server.dir/controller/register.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/an/Documents/GitHub/BattelShip/controller/register.c > CMakeFiles/Server.dir/controller/register.c.i

CMakeFiles/Server.dir/controller/register.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server.dir/controller/register.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/an/Documents/GitHub/BattelShip/controller/register.c -o CMakeFiles/Server.dir/controller/register.c.s

CMakeFiles/Server.dir/controller/login.c.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/controller/login.c.o: ../controller/login.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/an/Documents/GitHub/BattelShip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/Server.dir/controller/login.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Server.dir/controller/login.c.o   -c /home/an/Documents/GitHub/BattelShip/controller/login.c

CMakeFiles/Server.dir/controller/login.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Server.dir/controller/login.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/an/Documents/GitHub/BattelShip/controller/login.c > CMakeFiles/Server.dir/controller/login.c.i

CMakeFiles/Server.dir/controller/login.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Server.dir/controller/login.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/an/Documents/GitHub/BattelShip/controller/login.c -o CMakeFiles/Server.dir/controller/login.c.s

# Object files for target Server
Server_OBJECTS = \
"CMakeFiles/Server.dir/server.c.o" \
"CMakeFiles/Server.dir/model/auth.c.o" \
"CMakeFiles/Server.dir/model/network.c.o" \
"CMakeFiles/Server.dir/view/base_view.c.o" \
"CMakeFiles/Server.dir/view/login_view.c.o" \
"CMakeFiles/Server.dir/view/register_view.c.o" \
"CMakeFiles/Server.dir/view/menu_view.c.o" \
"CMakeFiles/Server.dir/controller/register.c.o" \
"CMakeFiles/Server.dir/controller/login.c.o"

# External object files for target Server
Server_EXTERNAL_OBJECTS =

Server: CMakeFiles/Server.dir/server.c.o
Server: CMakeFiles/Server.dir/model/auth.c.o
Server: CMakeFiles/Server.dir/model/network.c.o
Server: CMakeFiles/Server.dir/view/base_view.c.o
Server: CMakeFiles/Server.dir/view/login_view.c.o
Server: CMakeFiles/Server.dir/view/register_view.c.o
Server: CMakeFiles/Server.dir/view/menu_view.c.o
Server: CMakeFiles/Server.dir/controller/register.c.o
Server: CMakeFiles/Server.dir/controller/login.c.o
Server: CMakeFiles/Server.dir/build.make
Server: CMakeFiles/Server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/an/Documents/GitHub/BattelShip/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking C executable Server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Server.dir/build: Server

.PHONY : CMakeFiles/Server.dir/build

CMakeFiles/Server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Server.dir/clean

CMakeFiles/Server.dir/depend:
	cd /home/an/Documents/GitHub/BattelShip/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/an/Documents/GitHub/BattelShip /home/an/Documents/GitHub/BattelShip /home/an/Documents/GitHub/BattelShip/build /home/an/Documents/GitHub/BattelShip/build /home/an/Documents/GitHub/BattelShip/build/CMakeFiles/Server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Server.dir/depend
