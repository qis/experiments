//#include <common.hpp>
#include <boost/describe.hpp>
#include <boost/mp11.hpp>

namespace experiments::example {

// clang-format off
BOOST_DEFINE_ENUM_CLASS(solution,
  simple,
  optimized);
// clang-format on

template <solution Solution>
int run()
{
  return -1;
}

}  // namespace experiments::example

#include <iomanip>
#include <iostream>

void test()
{
  using namespace experiments::example;
  boost::mp11::mp_for_each<boost::describe::describe_enumerators<solution>>([&](auto solution) {
    std::cout << solution.name << ": " << run<solution.value>() << std::endl;
  });
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
  using namespace experiments::example;
  for (auto _ : state) {
    auto result = run<Solution>();
    benchmark::DoNotOptimize(result);
  }
}

BENCHMARK_TEMPLATE(experiments_example_run, solution::simple);
BENCHMARK_TEMPLATE(experiments_example_run, solution::optimized);

#endif  // ENABLE_BENCHMARKS
