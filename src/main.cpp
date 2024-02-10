#include "main.hpp"
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[])
{
  try {
    test();
  }
  catch (const std::exception& e) {
    std::fputs(e.what(), stderr);
    std::fputc('\n', stderr);
    std::fflush(stderr);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
