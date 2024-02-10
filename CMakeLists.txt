cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(experiments VERSION 0.1.0 LANGUAGES CXX)

# Executable
file(GLOB headers src/*.hpp)
file(GLOB sources src/*.cpp)

add_executable(main ${headers} ${sources} src/main.manifest src/main.rc)
set_target_properties(main PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
target_compile_features(main PRIVATE cxx_std_23)
target_include_directories(main PRIVATE src)

find_package(Boost REQUIRED COMPONENTS headers json)
target_link_libraries(main PRIVATE Boost::headers Boost::json)

if(MSVC)
  target_compile_options(main PRIVATE /arch:AVX2)
else()
  target_compile_options(main PRIVATE -mavx2)
endif()

if(WIN32)
  target_compile_definitions(main PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()
