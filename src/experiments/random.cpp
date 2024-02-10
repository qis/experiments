#include "common.hpp"
#include <format>
#include <random>
#include <string>

namespace experiments {

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

}  // namespace experiments

static void random(benchmark::State& state)
{
  for (auto _ : state) {
    auto str = experiments::random();
    benchmark::DoNotOptimize(str);
  }
}
BENCHMARK(random);

TEST_CASE("experiments::random")
{
  REQUIRE(!experiments::random().empty());
}
