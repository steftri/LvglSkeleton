cmake_minimum_required(VERSION 3.14)

project(LvglSkeletonProject LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the example application
add_executable(LvglSkeletonApp 
                hal/main_plain/main.cpp
                hal/datastorage_file/datastorage.cpp)

# Include directories
target_include_directories(LvglSkeletonApp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
                hal/main_plain
                hal/datastorage_file)

# Link the library to the example application
target_link_libraries(LvglSkeletonApp PRIVATE LvglSkeleton)





# GoogleTest unit and integration tests
#
# Check if the current project is the main project being built.
# This ensures that tests are only included when this project is not a subproject.
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  include(CTest)

  if(BUILD_TESTING)
    find_package(GTest REQUIRED)

    add_subdirectory(test/test_unit_crc32_gtest)
    add_subdirectory(test/test_unit_data_gtest)
    #add_subdirectory(test/test_integration_gtest)

    find_package(Threads REQUIRED)

    # Ensure test executables link pthreads on platforms where gtest does not add it transitively.
    target_link_libraries(UnitTestCrc32 PRIVATE Threads::Threads)
    target_link_libraries(UnitTestData PRIVATE Threads::Threads)
    #target_link_libraries(IntegrationTests PRIVATE Threads::Threads)

    add_custom_target(UnitTests
      COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
      DEPENDS UnitTestCrc32 UnitTestData
      COMMENT "Building and running all unit tests"
    )
  endif()

endif()



# Add a target to optionally generate Doxygen documentation
find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
  add_custom_target(Doxygen
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/LvglSkeleton
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM
  )
else()
  message(STATUS "Doxygen not found. Documentation target will not be available.")
endif()
