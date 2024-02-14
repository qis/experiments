//#include <common.hpp>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>

namespace experiments::example {

// =====================================================================================================================
// Definitions
// =====================================================================================================================

int g_number = -1;

// =====================================================================================================================
// Solutions
// =====================================================================================================================

// clang-format off
BOOST_DEFINE_ENUM_CLASS(solution,
  simple,
  optimized);
// clang-format on

template <solution Solution>
int run()
{
  return g_number;
}

// =====================================================================================================================
// Helpers
// =====================================================================================================================


}  // namespace experiments::example

#include <iostream>

void test()
{
  using namespace experiments::example;
  std::cout << run<solution::simple>() << std::endl;
}

#if ENABLE_TESTS

TEST_CASE("experiments::example::run")
{
  using namespace experiments::example;
  boost::mp11::mp_for_each<boost::describe::describe_enumerators<solution>>([&](auto solution) {
    SUBCASE(solution.name);
    REQUIRE(run<solution.value>() == -1);
  });
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS

using experiments::example::solution;

template <solution Solution>
void experiments_example_run(benchmark::State& state)
{
  for (auto _ : state) {
    auto result = experiments::example::run<Solution>();
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_TEMPLATE(experiments_example_run, solution::simple);
BENCHMARK_TEMPLATE(experiments_example_run, solution::optimized);

#endif  // ENABLE_BENCHMARKS
