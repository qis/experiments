cmake_minimum_required(VERSION 3.23 FATAL_ERROR)
project(experiments VERSION 0.1.0 LANGUAGES CXX)

# Conan
string(TOLOWER "third_party/${CMAKE_SYSTEM_NAME}" third_party)
if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${third_party}/bin)
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${third_party}/bin/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

list(PREPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${third_party})
list(PREPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${third_party})

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
else()
  target_compile_definitions(main PRIVATE -D_LIBCPP_ENABLE_EXPERIMENTAL=1)
  target_compile_options(main PRIVATE -stdlib=libc++)
  target_link_libraries(main PRIVATE c++experimental)
  target_link_libraries(main PRIVATE c++)
endif()
