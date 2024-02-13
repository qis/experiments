cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(experiments VERSION 0.1.0 LANGUAGES CXX)

# Main
add_executable(main src/main.cpp src/main.manifest src/main.rc)
set_target_properties(main PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# Objects
file(GLOB_RECURSE sources CONFIGURE_DEPENDS
  src/linked_lists/*.[hc]pp
  src/binary_search/*.[hc]pp
  src/hash_table/*.[hc]pp)

add_library(objects OBJECT ${sources})

# Properties
foreach(name main objects)
  target_compile_features(${name} PUBLIC cxx_std_23)
  target_include_directories(${name} PUBLIC src)

  if(MSVC)
    target_compile_options(${name} PUBLIC /arch:AVX2)
  else()
    target_compile_options(${name} PUBLIC -mavx2)
  endif()

  if(WIN32)
    target_compile_definitions(${name} PUBLIC NOMINMAX WIN32_LEAN_AND_MEAN)
  endif()
endforeach()

# Dependencies
find_package(Boost REQUIRED COMPONENTS headers json)
target_link_libraries(main PRIVATE Boost::headers Boost::json)
target_link_libraries(objects PUBLIC Boost::headers Boost::json)

find_package(TBB REQUIRED COMPONENTS tbb)
target_link_libraries(main PRIVATE TBB::tbb)
target_link_libraries(objects PUBLIC TBB::tbb)

find_package(doctest CONFIG REQUIRED)
target_link_libraries(objects PUBLIC doctest::doctest)

find_package(benchmark CONFIG REQUIRED)
target_link_libraries(objects PUBLIC benchmark::benchmark)

# Tests
add_executable(tests src/tests.cpp src/main.manifest)
target_link_libraries(tests PRIVATE objects)

# Benchmarks
add_executable(benchmarks src/benchmarks.cpp src/main.manifest)
target_link_libraries(benchmarks PRIVATE objects)
