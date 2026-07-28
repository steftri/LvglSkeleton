cmake_minimum_required(VERSION 3.14)

project(CrcLibrary LANGUAGES CXX)

add_library(CrcLibrary STATIC
  src/crc32.cpp
)

# Set C++ standard for this target
target_compile_features(CrcLibrary PUBLIC cxx_std_17)

# Add compiler options
target_compile_options(CrcLibrary PRIVATE -Wall -Werror -pedantic)

# Include directories
target_include_directories(CrcLibrary PUBLIC src)

# Set architecture (required for Apple silicon)
set(CMAKE_OSX_ARCHITECTURES "arm64")

# Installation rules (optional)
install(TARGETS CrcLibrary
  EXPORT CrcLibraryTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY src/
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
)

# Export rules (optional)
export(TARGETS CrcLibrary FILE CrcLibraryConfig.cmake)
