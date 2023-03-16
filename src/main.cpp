#include <boost/json.hpp>
#include <format>
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[])
{
  try {
    boost::json::object v;
    v["one"] = 1;
    v["two"] = 2;
    std::puts(boost::json::serialize(v).data());
  }
  catch (const std::exception& e) {
    std::fputs(e.what(), stderr);
    std::fputc('\n', stderr);
    std::fflush(stderr);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
