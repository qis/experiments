#include <common.hpp>
#include <format>
#include <random>
#include <string>

namespace algorithms {

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

}  // namespace algorithms

static void random(benchmark::State& state)
{
  for (auto _ : state) {
    auto str = algorithms::random();
    benchmark::DoNotOptimize(str);
  }
}
BENCHMARK(random);

TEST_CASE("algorithms::random")
{
  REQUIRE(!algorithms::random().empty());
}
