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

# Include any dependencies generated for this target.
include esp-idf/cxx/CMakeFiles/__idf_cxx.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include esp-idf/cxx/CMakeFiles/__idf_cxx.dir/compiler_depend.make

# Include the progress variables for this target.
include esp-idf/cxx/CMakeFiles/__idf_cxx.dir/progress.make

# Include the compile flags for this target's objects.
include esp-idf/cxx/CMakeFiles/__idf_cxx.dir/flags.make

esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_exception_stubs.cpp.obj: esp-idf/cxx/CMakeFiles/__idf_cxx.dir/flags.make
esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_exception_stubs.cpp.obj: C:/Users/gil18/esp/esp-idf/components/cxx/cxx_exception_stubs.cpp
esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_exception_stubs.cpp.obj: esp-idf/cxx/CMakeFiles/__idf_cxx.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\gil18\Downloads\ECEN404\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_exception_stubs.cpp.obj"
	cd /d C:\Users\gil18\Downloads\ECEN404\build\esp-idf\cxx && C:\Downloads\tools\xtensa-esp32-elf\esp-2022r1-11.2.0\xtensa-esp32-elf\bin\xtensa-esp32-elf-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_exception_stubs.cpp.obj -MF CMakeFiles\__idf_cxx.dir\cxx_exception_stubs.cpp.obj.d -o CMakeFiles\__idf_cxx.dir\cxx_exception_stubs.cpp.obj -c C:\Users\gil18\esp\esp-idf\components\cxx\cxx_exception_stubs.cpp

esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_exception_stubs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/__idf_cxx.dir/cxx_exception_stubs.cpp.i"
	cd /d C:\Users\gil18\Downloads\ECEN404\build\esp-idf\cxx && C:\Downloads\tools\xtensa-esp32-elf\esp-2022r1-11.2.0\xtensa-esp32-elf\bin\xtensa-esp32-elf-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\gil18\esp\esp-idf\components\cxx\cxx_exception_stubs.cpp > CMakeFiles\__idf_cxx.dir\cxx_exception_stubs.cpp.i

esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_exception_stubs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/__idf_cxx.dir/cxx_exception_stubs.cpp.s"
	cd /d C:\Users\gil18\Downloads\ECEN404\build\esp-idf\cxx && C:\Downloads\tools\xtensa-esp32-elf\esp-2022r1-11.2.0\xtensa-esp32-elf\bin\xtensa-esp32-elf-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\gil18\esp\esp-idf\components\cxx\cxx_exception_stubs.cpp -o CMakeFiles\__idf_cxx.dir\cxx_exception_stubs.cpp.s

esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_guards.cpp.obj: esp-idf/cxx/CMakeFiles/__idf_cxx.dir/flags.make
esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_guards.cpp.obj: C:/Users/gil18/esp/esp-idf/components/cxx/cxx_guards.cpp
esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_guards.cpp.obj: esp-idf/cxx/CMakeFiles/__idf_cxx.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\gil18\Downloads\ECEN404\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_guards.cpp.obj"
	cd /d C:\Users\gil18\Downloads\ECEN404\build\esp-idf\cxx && C:\Downloads\tools\xtensa-esp32-elf\esp-2022r1-11.2.0\xtensa-esp32-elf\bin\xtensa-esp32-elf-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_guards.cpp.obj -MF CMakeFiles\__idf_cxx.dir\cxx_guards.cpp.obj.d -o CMakeFiles\__idf_cxx.dir\cxx_guards.cpp.obj -c C:\Users\gil18\esp\esp-idf\components\cxx\cxx_guards.cpp

esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_guards.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/__idf_cxx.dir/cxx_guards.cpp.i"
	cd /d C:\Users\gil18\Downloads\ECEN404\build\esp-idf\cxx && C:\Downloads\tools\xtensa-esp32-elf\esp-2022r1-11.2.0\xtensa-esp32-elf\bin\xtensa-esp32-elf-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\gil18\esp\esp-idf\components\cxx\cxx_guards.cpp > CMakeFiles\__idf_cxx.dir\cxx_guards.cpp.i

esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_guards.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/__idf_cxx.dir/cxx_guards.cpp.s"
	cd /d C:\Users\gil18\Downloads\ECEN404\build\esp-idf\cxx && C:\Downloads\tools\xtensa-esp32-elf\esp-2022r1-11.2.0\xtensa-esp32-elf\bin\xtensa-esp32-elf-g++.exe $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\gil18\esp\esp-idf\components\cxx\cxx_guards.cpp -o CMakeFiles\__idf_cxx.dir\cxx_guards.cpp.s

# Object files for target __idf_cxx
__idf_cxx_OBJECTS = \
"CMakeFiles/__idf_cxx.dir/cxx_exception_stubs.cpp.obj" \
"CMakeFiles/__idf_cxx.dir/cxx_guards.cpp.obj"

# External object files for target __idf_cxx
__idf_cxx_EXTERNAL_OBJECTS =

esp-idf/cxx/libcxx.a: esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_exception_stubs.cpp.obj
esp-idf/cxx/libcxx.a: esp-idf/cxx/CMakeFiles/__idf_cxx.dir/cxx_guards.cpp.obj
esp-idf/cxx/libcxx.a: esp-idf/cxx/CMakeFiles/__idf_cxx.dir/build.make
esp-idf/cxx/libcxx.a: esp-idf/cxx/CMakeFiles/__idf_cxx.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\gil18\Downloads\ECEN404\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C static library libcxx.a"
	cd /d C:\Users\gil18\Downloads\ECEN404\build\esp-idf\cxx && $(CMAKE_COMMAND) -P CMakeFiles\__idf_cxx.dir\cmake_clean_target.cmake
	cd /d C:\Users\gil18\Downloads\ECEN404\build\esp-idf\cxx && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\__idf_cxx.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
esp-idf/cxx/CMakeFiles/__idf_cxx.dir/build: esp-idf/cxx/libcxx.a
.PHONY : esp-idf/cxx/CMakeFiles/__idf_cxx.dir/build

esp-idf/cxx/CMakeFiles/__idf_cxx.dir/clean:
	cd /d C:\Users\gil18\Downloads\ECEN404\build\esp-idf\cxx && $(CMAKE_COMMAND) -P CMakeFiles\__idf_cxx.dir\cmake_clean.cmake
.PHONY : esp-idf/cxx/CMakeFiles/__idf_cxx.dir/clean

esp-idf/cxx/CMakeFiles/__idf_cxx.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\gil18\Downloads\ECEN404 C:\Users\gil18\esp\esp-idf\components\cxx C:\Users\gil18\Downloads\ECEN404\build C:\Users\gil18\Downloads\ECEN404\build\esp-idf\cxx C:\Users\gil18\Downloads\ECEN404\build\esp-idf\cxx\CMakeFiles\__idf_cxx.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : esp-idf/cxx/CMakeFiles/__idf_cxx.dir/depend

