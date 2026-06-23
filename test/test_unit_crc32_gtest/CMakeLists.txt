# CMake configuration for unit tests
add_executable(UnitTestCrc32 test_unit_crc32.cpp)

# Set C++ standard for this target
target_compile_features(UnitTestCrc32 PUBLIC cxx_std_17)

# Add the CRC library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/crc ${CMAKE_BINARY_DIR}/lib_crc)

# Link the library to the unit tests
target_link_libraries(UnitTestCrc32 PRIVATE CrcLibrary GTest::GTest GTest::Main)

# Enable testing for this target
add_test(NAME UnitTestCrc32 COMMAND UnitTestCrc32)
