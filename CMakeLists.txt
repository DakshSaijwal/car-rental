cmake_minimum_required(VERSION 3.14)
project(CarRentalSystem CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set strict warning flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")
endif()

# Include directories
include_directories(include)

# Find all source files except main.cpp for the library
file(GLOB_RECURSE LIB_SOURCES "src/models/*.cpp" "src/services/*.cpp")

# Define library for testing
add_library(rental_lib ${LIB_SOURCES})

# Define main executable
add_executable(rental src/main.cpp)
target_link_libraries(rental PRIVATE rental_lib)

# --- Google Test Configuration ---
include(FetchContent)
set(CMAKE_TLS_VERIFY OFF) # Suppress SSL certificate errors on some Windows environments
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  rental_tests
  tests/test_rental_service.cpp
)
target_link_libraries(
  rental_tests
  GTest::gtest_main
  rental_lib
)

include(GoogleTest)
gtest_discover_tests(rental_tests)
