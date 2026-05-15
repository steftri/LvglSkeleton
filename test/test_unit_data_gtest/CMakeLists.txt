# CMake configuration for unit tests
add_executable(UnitTestData test_unit_data.cpp)

# Set C++ standard for this target
target_compile_features(UnitTestData PUBLIC cxx_std_17)

# Add the data library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/data ${CMAKE_BINARY_DIR}/lib_data)

# Link the library to the unit tests
target_link_libraries(UnitTestData PRIVATE DataLibrary GTest::GTest GTest::Main)

# Enable testing for this target
add_test(NAME UnitTestData COMMAND UnitTestData)
