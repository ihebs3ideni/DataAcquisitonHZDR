# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2022.1.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2022.1.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\User\Desktop\HZDR\hzdr_tasler_data_acquisition

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\User\Desktop\HZDR\hzdr_tasler_data_acquisition\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/HZDR_Data_Acquisition.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/HZDR_Data_Acquisition.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/HZDR_Data_Acquisition.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HZDR_Data_Acquisition.dir/flags.make

CMakeFiles/HZDR_Data_Acquisition.dir/main.cpp.obj: CMakeFiles/HZDR_Data_Acquisition.dir/flags.make
CMakeFiles/HZDR_Data_Acquisition.dir/main.cpp.obj: CMakeFiles/HZDR_Data_Acquisition.dir/includes_CXX.rsp
CMakeFiles/HZDR_Data_Acquisition.dir/main.cpp.obj: ../main.cpp
CMakeFiles/HZDR_Data_Acquisition.dir/main.cpp.obj: CMakeFiles/HZDR_Data_Acquisition.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\User\Desktop\HZDR\hzdr_tasler_data_acquisition\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HZDR_Data_Acquisition.dir/main.cpp.obj"
	C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HZDR_Data_Acquisition.dir/main.cpp.obj -MF CMakeFiles\HZDR_Data_Acquisition.dir\main.cpp.obj.d -o CMakeFiles\HZDR_Data_Acquisition.dir\main.cpp.obj -c C:\Users\User\Desktop\HZDR\hzdr_tasler_data_acquisition\main.cpp

CMakeFiles/HZDR_Data_Acquisition.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HZDR_Data_Acquisition.dir/main.cpp.i"
	C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\User\Desktop\HZDR\hzdr_tasler_data_acquisition\main.cpp > CMakeFiles\HZDR_Data_Acquisition.dir\main.cpp.i

CMakeFiles/HZDR_Data_Acquisition.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HZDR_Data_Acquisition.dir/main.cpp.s"
	C:\TDM-GCC-32\bin\g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\User\Desktop\HZDR\hzdr_tasler_data_acquisition\main.cpp -o CMakeFiles\HZDR_Data_Acquisition.dir\main.cpp.s

# Object files for target HZDR_Data_Acquisition
HZDR_Data_Acquisition_OBJECTS = \
"CMakeFiles/HZDR_Data_Acquisition.dir/main.cpp.obj"

# External object files for target HZDR_Data_Acquisition
HZDR_Data_Acquisition_EXTERNAL_OBJECTS =

HZDR_Data_Acquisition.exe: CMakeFiles/HZDR_Data_Acquisition.dir/main.cpp.obj
HZDR_Data_Acquisition.exe: CMakeFiles/HZDR_Data_Acquisition.dir/build.make
HZDR_Data_Acquisition.exe: CMakeFiles/HZDR_Data_Acquisition.dir/linklibs.rsp
HZDR_Data_Acquisition.exe: CMakeFiles/HZDR_Data_Acquisition.dir/objects1.rsp
HZDR_Data_Acquisition.exe: CMakeFiles/HZDR_Data_Acquisition.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\User\Desktop\HZDR\hzdr_tasler_data_acquisition\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable HZDR_Data_Acquisition.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\HZDR_Data_Acquisition.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HZDR_Data_Acquisition.dir/build: HZDR_Data_Acquisition.exe
.PHONY : CMakeFiles/HZDR_Data_Acquisition.dir/build

CMakeFiles/HZDR_Data_Acquisition.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\HZDR_Data_Acquisition.dir\cmake_clean.cmake
.PHONY : CMakeFiles/HZDR_Data_Acquisition.dir/clean

CMakeFiles/HZDR_Data_Acquisition.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\User\Desktop\HZDR\hzdr_tasler_data_acquisition C:\Users\User\Desktop\HZDR\hzdr_tasler_data_acquisition C:\Users\User\Desktop\HZDR\hzdr_tasler_data_acquisition\cmake-build-debug C:\Users\User\Desktop\HZDR\hzdr_tasler_data_acquisition\cmake-build-debug C:\Users\User\Desktop\HZDR\hzdr_tasler_data_acquisition\cmake-build-debug\CMakeFiles\HZDR_Data_Acquisition.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HZDR_Data_Acquisition.dir/depend
