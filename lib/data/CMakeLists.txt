cmake_minimum_required(VERSION 3.14)

project(DataLibrary LANGUAGES CXX)

add_library(DataLibrary STATIC
  src/data.cpp
)

# Set C++ standard for this target
target_compile_features(DataLibrary PUBLIC cxx_std_17)

# Add compiler options
target_compile_options(DataLibrary PRIVATE -Wall -Werror -pedantic)

# Include directories
target_include_directories(DataLibrary PUBLIC src)

# Set architecture (required for Apple silicon)
set(CMAKE_OSX_ARCHITECTURES "arm64")

# Installation rules (optional)
install(TARGETS DataLibrary
  EXPORT DataLibraryTargets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)
install(DIRECTORY src/
  DESTINATION include
  FILES_MATCHING PATTERN "*.h"
)

# Export rules (optional)
export(TARGETS DataLibrary FILE DataLibraryConfig.cmake)
