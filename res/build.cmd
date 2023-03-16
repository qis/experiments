@echo off
pushd %~dp0..

time /t

cmake --preset debug
cmake --preset release
cmake --preset debug-clang
cmake --preset release-clang
cmake --preset debug-clang-cl
cmake --preset release-clang-cl

cmake --build build/debug
cmake --build build/release
cmake --build build/debug-clang
cmake --build build/release-clang
cmake --build build/debug-clang-cl
cmake --build build/release-clang-cl

time /t

popd
