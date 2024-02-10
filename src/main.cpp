#include <format>
#include <random>
#include <string>

namespace application {

std::string random()
{
  static thread_local std::random_device rd;
  static thread_local std::uniform_int_distribution<std::size_t> dist(
    0,
    std::numeric_limits<std::size_t>::max());

  std::string result;
  for (auto i = 0; i < 1000; i++) {
    result = std::format("{}", dist(rd));
  }
  return result;
}

}  // namespace application

int main(int argc, char* argv[])
{
  try {
    std::puts(std::format("{}\n", application::random()).data());
  }
  catch (const std::exception& e) {
    std::fputs(e.what(), stderr);
    std::fputc('\n', stderr);
    std::fflush(stderr);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
