#include "main.hpp"
#include <format>
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[])
{
  try {
    std::puts(std::format("{}", test()).data());
  }
  catch (const std::exception& e) {
    std::fputs(e.what(), stderr);
    std::fputc('\n', stderr);
    std::fflush(stderr);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
