# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = C:\Downloads\tools\cmake\3.24.0\bin\cmake.exe

# The command to remove a file.
RM = C:\Downloads\tools\cmake\3.24.0\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\gil18\Downloads\ECEN404

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\gil18\Downloads\ECEN404\build

# Utility rule file for app-flash.

# Include any custom commands dependencies for this target.
include esp-idf/esptool_py/CMakeFiles/app-flash.dir/compiler_depend.make

# Include the progress variables for this target.
include esp-idf/esptool_py/CMakeFiles/app-flash.dir/progress.make

esp-idf/esptool_py/CMakeFiles/app-flash:
	cd /d C:\Users\gil18\esp\esp-idf\components\esptool_py && C:\Downloads\tools\cmake\3.24.0\bin\cmake.exe -D IDF_PATH=C:/Users/gil18/esp/esp-idf -D SERIAL_TOOL=c:/Downloads/python_env/idf5.0_py3.8_env/Scripts/python.exe;;C:/Users/gil18/esp/esp-idf/components/esptool_py/esptool/esptool.py;--chip;esp32 -D SERIAL_TOOL_ARGS=--before=default_reset;--after=hard_reset;write_flash;@app-flash_args -D WORKING_DIRECTORY=C:/Users/gil18/Downloads/ECEN404/build -P C:/Users/gil18/esp/esp-idf/components/esptool_py/run_serial_tool.cmake

app-flash: esp-idf/esptool_py/CMakeFiles/app-flash
app-flash: esp-idf/esptool_py/CMakeFiles/app-flash.dir/build.make
.PHONY : app-flash

# Rule to build all files generated by this target.
esp-idf/esptool_py/CMakeFiles/app-flash.dir/build: app-flash
.PHONY : esp-idf/esptool_py/CMakeFiles/app-flash.dir/build

esp-idf/esptool_py/CMakeFiles/app-flash.dir/clean:
	cd /d C:\Users\gil18\Downloads\ECEN404\build\esp-idf\esptool_py && $(CMAKE_COMMAND) -P CMakeFiles\app-flash.dir\cmake_clean.cmake
.PHONY : esp-idf/esptool_py/CMakeFiles/app-flash.dir/clean

esp-idf/esptool_py/CMakeFiles/app-flash.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\gil18\Downloads\ECEN404 C:\Users\gil18\esp\esp-idf\components\esptool_py C:\Users\gil18\Downloads\ECEN404\build C:\Users\gil18\Downloads\ECEN404\build\esp-idf\esptool_py C:\Users\gil18\Downloads\ECEN404\build\esp-idf\esptool_py\CMakeFiles\app-flash.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : esp-idf/esptool_py/CMakeFiles/app-flash.dir/depend

