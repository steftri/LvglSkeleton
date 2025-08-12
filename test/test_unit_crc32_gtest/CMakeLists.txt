# CMake configuration for unit tests
add_executable(UnitTests test_unit_crc32.cpp)

# Set C++ standard for this target
target_compile_features(UnitTests PUBLIC cxx_std_17)

# Add the CRC library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/crc ${CMAKE_BINARY_DIR}/lib_crc)

# Link the library to the unit tests
target_link_libraries(UnitTests PRIVATE CrcLibrary GTest::GTest GTest::Main)

# Enable testing for this target
add_test(NAME UnitTests COMMAND UnitTests)
