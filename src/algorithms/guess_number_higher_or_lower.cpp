// https://leetcode.com/problems/guess-number-higher-or-lower/
// -------------------------------------------------------------------------------------------------------------------
// Benchmark                                                                              Time        CPU   Iterations
// -------------------------------------------------------------------------------------------------------------------
// algorithms_guess_number_higher_or_lower_simple/123/2147483646                       27.5 ns    27.0 ns     24888889
// algorithms_guess_number_higher_or_lower_integer_range/123/2147483646                14.2 ns    14.1 ns     49777778
// algorithms_guess_number_higher_or_lower_const_integer_range/123/2147483646          9.23 ns    9.00 ns     74666667
// algorithms_guess_number_higher_or_lower_bidirectional/123/2147483646                12.2 ns    11.7 ns     56000000
// algorithms_guess_number_higher_or_lower_const_bidirectional/123/2147483646          11.9 ns    11.2 ns     56000000
// algorithms_guess_number_higher_or_lower_random_access/123/2147483646                16.5 ns    16.4 ns     44800000
// algorithms_guess_number_higher_or_lower_const_random_access/123/2147483646          16.5 ns    16.4 ns     44800000
// algorithms_guess_number_higher_or_lower_legacy_random_access/123/2147483646         8.73 ns    8.79 ns    112000000
// algorithms_guess_number_higher_or_lower_const_legacy_random_access/123/2147483646   8.87 ns    8.79 ns     74666667

#include <boost/range/irange.hpp>
#include <common.hpp>
#include <algorithm>
#include <compare>
#include <iterator>
#include <limits>
#include <utility>
#include <cassert>

namespace algorithms::guess_number_higher_or_lower {

// =====================================================================================================================
// Definitions
// =====================================================================================================================

int guess(int num);

// =====================================================================================================================
// Solutions
// =====================================================================================================================

int simple(int n) noexcept
{
  auto lhs = 1;
  auto rhs = n;
  while (true) {
    const auto i = lhs + (rhs - lhs) / 2;
    if (const auto r = guess(i); r < 0) {
      rhs = i - 1;
    } else if (r > 0) {
      lhs = i + 1;
    } else {
      return i;
    }
  }
  std::unreachable();
  return -1;
}

int integer_range(int n) noexcept
{
  const auto range = boost::integer_range<int>(1, n);
  return *std::lower_bound(range.begin(), range.end(), 0, [](int lhs, int) noexcept {
    return guess(lhs) > 0;
  });
}

int const_integer_range(int n) noexcept
{
  const auto range = boost::integer_range<const int>(1, n);
  return *std::lower_bound(range.begin(), range.end(), 0, [](int lhs, int) noexcept {
    return guess(lhs) > 0;
  });
}

// NOTE: The std::lower_bound algorithm is optimized for log₂(last - first) + O(1) number of
//       comparisons for iterators that satisfy the LegacyRandomAccessIterator requirement.

namespace legacy {  // clang-format off

template <class T>
using with_reference = T&;

template <class T>
concept referenceable = requires {
  typename with_reference<T>;
};

template <class I>
concept iterator =
  requires(I i) {
    {   *i } -> referenceable;
    {  ++i } -> std::same_as<I&>;
    { *i++ } -> referenceable;
  } && std::copyable<I>;


template <class I>
concept input_iterator =
  iterator<I> && std::equality_comparable<I> && requires(I i) {
    typename std::incrementable_traits<I>::difference_type;
    typename std::indirectly_readable_traits<I>::value_type;
    typename std::common_reference_t<std::iter_reference_t<I>&&,
      typename std::indirectly_readable_traits<I>::value_type&>;
    *i++;
    typename std::common_reference_t<decltype(*i++)&&, typename std::indirectly_readable_traits<I>::value_type&>;
    requires std::signed_integral<typename std::incrementable_traits<I>::difference_type>;
  };

template <class I>
concept forward_iterator =
  input_iterator<I> && std::constructible_from<I> &&
  std::is_reference_v<std::iter_reference_t<I>> &&
  std::same_as<
    std::remove_cvref_t<std::iter_reference_t<I>>,
    typename std::indirectly_readable_traits<I>::value_type> &&
  requires(I i) {
    {  i++ } -> std::convertible_to<const I&>;
    { *i++ } -> std::same_as<std::iter_reference_t<I>>;
  };

template <class I>
concept bidirectional_iterator =
  forward_iterator<I> && requires(I i) {
    {  --i } -> std::same_as<I&>;
    {  i-- } -> std::convertible_to<const I&>;
    { *i-- } -> std::same_as<std::iter_reference_t<I>>;
  };

template <class I>
concept random_access_iterator =
  bidirectional_iterator<I> && std::totally_ordered<I> &&
  requires(I i, typename std::incrementable_traits<I>::difference_type n) {
    { i += n } -> std::same_as<I&>;
    { i -= n } -> std::same_as<I&>;
    { i +  n } -> std::same_as<I>;
    { n +  i } -> std::same_as<I>;
    { i -  n } -> std::same_as<I>;
    { i -  i } -> std::same_as<decltype(n)>;
    {  i[n]  } -> std::convertible_to<std::iter_reference_t<I>>;
  };

}  // clang-format on

class bidirectional_iterator {
public:
  using value_type = int;
  using reference = int;
  using difference_type = int;
  using iterator = bidirectional_iterator;

  constexpr bidirectional_iterator() noexcept = default;
  constexpr bidirectional_iterator(int value) noexcept : value_(value) {}
  constexpr bidirectional_iterator(bidirectional_iterator&& other) noexcept = default;
  constexpr bidirectional_iterator(const bidirectional_iterator& other) noexcept = default;
  constexpr bidirectional_iterator& operator=(bidirectional_iterator&& other) noexcept = default;
  constexpr bidirectional_iterator& operator=(const bidirectional_iterator& other) noexcept = default;

  // clang-format off
  constexpr reference operator*() const noexcept { return value_; }
  constexpr iterator& operator++() noexcept { value_ += 1; return *this; }
  constexpr iterator operator++(int) noexcept { const iterator rv{ *this }; ++(*this); return rv; }
  constexpr iterator& operator--() noexcept { value_ -= 1; return *this; }
  constexpr iterator operator--(int) noexcept { const iterator rv{ *this }; --(*this); return rv; }
  // clang-format on

  constexpr auto operator<=>(iterator const& other) const noexcept = default;

protected:
  value_type value_{ 0 };
};

static_assert(std::bidirectional_iterator<bidirectional_iterator>);
static_assert(!std::random_access_iterator<bidirectional_iterator>);
static_assert(!legacy::random_access_iterator<bidirectional_iterator>);

class const_bidirectional_iterator {
public:
  using value_type = int;
  using reference = const int;
  using difference_type = int;
  using iterator = const_bidirectional_iterator;

  constexpr const_bidirectional_iterator() noexcept = default;
  constexpr const_bidirectional_iterator(int value) noexcept : value_(value) {}
  constexpr const_bidirectional_iterator(const_bidirectional_iterator&& other) noexcept = default;
  constexpr const_bidirectional_iterator(const const_bidirectional_iterator& other) noexcept = default;
  constexpr const_bidirectional_iterator& operator=(const_bidirectional_iterator&& other) noexcept = default;
  constexpr const_bidirectional_iterator& operator=(const const_bidirectional_iterator& other) noexcept = default;

  // clang-format off
  constexpr reference operator*() const noexcept { return value_; }
  constexpr iterator& operator++() noexcept { value_ += 1; return *this; }
  constexpr iterator operator++(int) noexcept { const iterator rv{ *this }; ++(*this); return rv; }
  constexpr iterator& operator--() noexcept { value_ -= 1; return *this; }
  constexpr iterator operator--(int) noexcept { const iterator rv{ *this }; --(*this); return rv; }
  // clang-format on

  constexpr auto operator<=>(iterator const& other) const noexcept = default;

protected:
  value_type value_{ 0 };
};

static_assert(std::bidirectional_iterator<const_bidirectional_iterator>);
static_assert(!std::random_access_iterator<const_bidirectional_iterator>);
static_assert(!legacy::random_access_iterator<const_bidirectional_iterator>);

class random_access_iterator {
public:
  using value_type = int;
  using reference = int;
  using difference_type = int;
  using iterator = random_access_iterator;

  constexpr random_access_iterator() noexcept = default;
  constexpr random_access_iterator(int value) noexcept : value_(value) {}
  constexpr random_access_iterator(random_access_iterator&& other) noexcept = default;
  constexpr random_access_iterator(const random_access_iterator& other) noexcept = default;
  constexpr random_access_iterator& operator=(random_access_iterator&& other) noexcept = default;
  constexpr random_access_iterator& operator=(const random_access_iterator& other) noexcept = default;

  // clang-format off
  constexpr reference operator*() const noexcept { return value_; }
  constexpr iterator& operator++() noexcept { value_ += 1; return *this; }
  constexpr iterator operator++(int) noexcept { const iterator rv{ *this }; ++(*this); return rv; }
  constexpr iterator& operator--() noexcept { value_ -= 1; return *this; }
  constexpr iterator operator--(int) noexcept { const iterator rv{ *this }; --(*this); return rv; }
  constexpr iterator& operator+=(difference_type n) noexcept { value_ += n; return *this; }
  constexpr iterator& operator-=(difference_type n) noexcept { value_ -= n; return *this; }
  constexpr iterator operator+(difference_type n) const noexcept { return { value_ + n }; }
  constexpr iterator operator-(difference_type n) const noexcept { return { value_ - n }; }
  // clang-format on

  constexpr reference operator[](difference_type n) const noexcept
  {
    return value_ + n;
  }

  friend constexpr difference_type operator-(const iterator& i, const iterator& s) noexcept
  {
    return i.value_ - s.value_;
  }

  friend constexpr iterator operator+(const iterator& i, const iterator& s) noexcept
  {
    return { i.value_ - s.value_ };
  }

  constexpr auto operator<=>(iterator const& other) const noexcept = default;

private:
  value_type value_{ 0 };
};

static_assert(std::random_access_iterator<random_access_iterator>);
static_assert(!legacy::random_access_iterator<random_access_iterator>);

class const_random_access_iterator {
public:
  using value_type = int;
  using reference = const int;
  using difference_type = int;
  using iterator = const_random_access_iterator;

  constexpr const_random_access_iterator() noexcept = default;
  constexpr const_random_access_iterator(int value) noexcept : value_(value) {}
  constexpr const_random_access_iterator(const_random_access_iterator&& other) noexcept = default;
  constexpr const_random_access_iterator(const const_random_access_iterator& other) noexcept = default;
  constexpr const_random_access_iterator& operator=(const_random_access_iterator&& other) noexcept = default;
  constexpr const_random_access_iterator& operator=(const const_random_access_iterator& other) noexcept = default;

  // clang-format off
  constexpr reference operator*() const noexcept { return value_; }
  constexpr iterator& operator++() noexcept { value_ += 1; return *this; }
  constexpr iterator operator++(int) noexcept { const iterator rv{ *this }; ++(*this); return rv; }
  constexpr iterator& operator--() noexcept { value_ -= 1; return *this; }
  constexpr iterator operator--(int) noexcept { const iterator rv{ *this }; --(*this); return rv; }
  constexpr iterator& operator+=(difference_type n) noexcept { value_ += n; return *this; }
  constexpr iterator& operator-=(difference_type n) noexcept { value_ -= n; return *this; }
  constexpr iterator operator+(difference_type n) const noexcept { return { value_ + n }; }
  constexpr iterator operator-(difference_type n) const noexcept { return { value_ - n }; }
  // clang-format on

  constexpr reference operator[](difference_type n) const noexcept
  {
    return value_ + n;
  }

  friend constexpr difference_type operator-(const iterator& i, const iterator& s) noexcept
  {
    return i.value_ - s.value_;
  }

  friend constexpr iterator operator+(const iterator& i, const iterator& s) noexcept
  {
    return { i.value_ - s.value_ };
  }

  constexpr auto operator<=>(iterator const& other) const noexcept = default;

private:
  value_type value_{ 0 };
};

static_assert(std::random_access_iterator<const_random_access_iterator>);
static_assert(!legacy::random_access_iterator<const_random_access_iterator>);

class legacy_random_access_iterator {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = int;
  using reference = int&;
  using pointer = int*;
  using difference_type = int;
  using iterator = legacy_random_access_iterator;

  constexpr legacy_random_access_iterator() noexcept = default;
  constexpr legacy_random_access_iterator(int value) noexcept : value_(value) {}
  constexpr legacy_random_access_iterator(legacy_random_access_iterator&& other) noexcept = default;
  constexpr legacy_random_access_iterator(const legacy_random_access_iterator& other) noexcept = default;
  constexpr legacy_random_access_iterator& operator=(legacy_random_access_iterator&& other) noexcept = default;
  constexpr legacy_random_access_iterator& operator=(const legacy_random_access_iterator& other) noexcept = default;

  // clang-format off
  constexpr reference operator*() const noexcept { return value_; }
  constexpr iterator& operator++() noexcept { value_ += 1; return *this; }
  constexpr iterator operator++(int) noexcept { const iterator rv{ *this }; ++(*this); return rv; }
  constexpr iterator& operator--() noexcept { value_ -= 1; return *this; }
  constexpr iterator operator--(int) noexcept { const iterator rv{ *this }; --(*this); return rv; }
  constexpr iterator& operator+=(difference_type n) noexcept { value_ += n; return *this; }
  constexpr iterator& operator-=(difference_type n) noexcept { value_ -= n; return *this; }
  constexpr iterator operator+(difference_type n) const noexcept { return { value_ + n }; }
  constexpr iterator operator-(difference_type n) const noexcept { return { value_ - n }; }
  // clang-format on

  // There is no need to implement this function, since std::lower_bound doesn't use it.
  reference operator[](difference_type n) const noexcept;
  //{
  //  //assert(false);
  //  static thread_local value_type value;
  //  value = value_ + n;
  //  return value;
  //}

  friend constexpr difference_type operator-(const iterator& i, const iterator& s) noexcept
  {
    return i.value_ - s.value_;
  }

  friend constexpr iterator operator+(const iterator& i, const iterator& s) noexcept
  {
    return { i.value_ - s.value_ };
  }

  constexpr auto operator<=>(iterator const& other) const noexcept = default;

private:
  mutable value_type value_{ 0 };
};

static_assert(std::random_access_iterator<legacy_random_access_iterator>);
static_assert(legacy::random_access_iterator<legacy_random_access_iterator>);

class const_legacy_random_access_iterator {
public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = int;
  using reference = const int&;
  using pointer = const int*;
  using difference_type = int;
  using iterator = const_legacy_random_access_iterator;

  constexpr const_legacy_random_access_iterator() noexcept = default;
  constexpr const_legacy_random_access_iterator(int value) noexcept : value_(value) {}
  constexpr const_legacy_random_access_iterator(const_legacy_random_access_iterator&& other) noexcept = default;
  constexpr const_legacy_random_access_iterator(const const_legacy_random_access_iterator& other) noexcept = default;
  constexpr const_legacy_random_access_iterator& operator=(const_legacy_random_access_iterator&& other) noexcept = default;
  constexpr const_legacy_random_access_iterator& operator=(const const_legacy_random_access_iterator& other) noexcept = default;

  // clang-format off
  constexpr reference operator*() const noexcept { return value_; }
  constexpr iterator& operator++() noexcept { value_ += 1; return *this; }
  constexpr iterator operator++(int) noexcept { const iterator rv{ *this }; ++(*this); return rv; }
  constexpr iterator& operator--() noexcept { value_ -= 1; return *this; }
  constexpr iterator operator--(int) noexcept { const iterator rv{ *this }; --(*this); return rv; }
  constexpr iterator& operator+=(difference_type n) noexcept { value_ += n; return *this; }
  constexpr iterator& operator-=(difference_type n) noexcept { value_ -= n; return *this; }
  constexpr iterator operator+(difference_type n) const noexcept { return { value_ + n }; }
  constexpr iterator operator-(difference_type n) const noexcept { return { value_ - n }; }
  // clang-format on

  // There is no need to implement this function, since std::lower_bound doesn't use it.
  reference operator[](difference_type n) const noexcept;
  //{
  //  //assert(false);
  //  static thread_local value_type value;
  //  value = value_ + n;
  //  return value;
  //}

  friend constexpr difference_type operator-(const iterator& i, const iterator& s) noexcept
  {
    return i.value_ - s.value_;
  }

  friend constexpr iterator operator+(const iterator& i, const iterator& s) noexcept
  {
    return { i.value_ - s.value_ };
  }

  constexpr auto operator<=>(iterator const& other) const noexcept = default;

private:
  value_type value_{ 0 };
};

static_assert(std::random_access_iterator<const_legacy_random_access_iterator>);
static_assert(legacy::random_access_iterator<const_legacy_random_access_iterator>);

template <class Iterator>
int standard(int n) noexcept
{
  return *std::lower_bound(Iterator{ 1 }, Iterator{ n }, 0, [](int lhs, int) noexcept {
    return guess(lhs) > 0;
  });
}

int bidirectional(int n) noexcept
{
  return standard<bidirectional_iterator>(n);
}

int const_bidirectional(int n) noexcept
{
  return standard<const_bidirectional_iterator>(n);
}

int random_access(int n) noexcept
{
  return standard<random_access_iterator>(n);
}

int const_random_access(int n) noexcept
{
  return standard<const_random_access_iterator>(n);
}

int legacy_random_access(int n) noexcept
{
  return standard<legacy_random_access_iterator>(n);
}

int const_legacy_random_access(int n) noexcept
{
  return standard<const_legacy_random_access_iterator>(n);
}

// =====================================================================================================================
// Helpers
// =====================================================================================================================

static thread_local int number = 0;

int guess(int i)
{
  if (i > number) {
    return -1;
  } else if (i < number) {
    return 1;
  }
  return 0;
}

}  // namespace algorithms::guess_number_higher_or_lower

#if ENABLE_TESTS || 1

TEST_CASE("algorithms::guess_number_higher_or_lower::simple")
{
  for (auto i = 1; i <= 8; i++) {
    algorithms::guess_number_higher_or_lower::number = i;
    REQUIRE(algorithms::guess_number_higher_or_lower::simple(8) == i);
  }
}

TEST_CASE("algorithms::guess_number_higher_or_lower::integer_range")
{
  for (auto i = 1; i <= 8; i++) {
    algorithms::guess_number_higher_or_lower::number = i;
    REQUIRE(algorithms::guess_number_higher_or_lower::integer_range(8) == i);
  }
}

TEST_CASE("algorithms::guess_number_higher_or_lower::const_integer_range")
{
  for (auto i = 1; i <= 8; i++) {
    algorithms::guess_number_higher_or_lower::number = i;
    REQUIRE(algorithms::guess_number_higher_or_lower::const_integer_range(8) == i);
  }
}

TEST_CASE("algorithms::guess_number_higher_or_lower::bidirectional")
{
  for (auto i = 1; i <= 8; i++) {
    algorithms::guess_number_higher_or_lower::number = i;
    REQUIRE(algorithms::guess_number_higher_or_lower::bidirectional(8) == i);
  }
}

TEST_CASE("algorithms::guess_number_higher_or_lower::const_bidirectional")
{
  for (auto i = 1; i <= 8; i++) {
    algorithms::guess_number_higher_or_lower::number = i;
    REQUIRE(algorithms::guess_number_higher_or_lower::const_bidirectional(8) == i);
  }
}

TEST_CASE("algorithms::guess_number_higher_or_lower::random_access")
{
  for (auto i = 1; i <= 8; i++) {
    algorithms::guess_number_higher_or_lower::number = i;
    REQUIRE(algorithms::guess_number_higher_or_lower::random_access(8) == i);
  }
}

TEST_CASE("algorithms::guess_number_higher_or_lower::const_random_access")
{
  for (auto i = 1; i <= 8; i++) {
    algorithms::guess_number_higher_or_lower::number = i;
    REQUIRE(algorithms::guess_number_higher_or_lower::const_random_access(8) == i);
  }
}

TEST_CASE("algorithms::guess_number_higher_or_lower::legacy_random_access")
{
  for (auto i = 1; i <= 8; i++) {
    algorithms::guess_number_higher_or_lower::number = i;
    REQUIRE(algorithms::guess_number_higher_or_lower::legacy_random_access(8) == i);
  }
}

TEST_CASE("algorithms::guess_number_higher_or_lower::const_legacy_random_access")
{
  for (auto i = 1; i <= 8; i++) {
    algorithms::guess_number_higher_or_lower::number = i;
    REQUIRE(algorithms::guess_number_higher_or_lower::const_legacy_random_access(8) == i);
  }
}

#endif  // ENABLE_TESTS

#if ENABLE_BENCHMARKS || 1

static void algorithms_guess_number_higher_or_lower_simple(benchmark::State& state)
{
  algorithms::guess_number_higher_or_lower::number = state.range(0);
  for (auto _ : state) {
    auto result = algorithms::guess_number_higher_or_lower::simple(state.range(1));
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_guess_number_higher_or_lower_simple)
  ->Args({ 123, std::numeric_limits<int>::max() - 1 });
// clang-format on

static void algorithms_guess_number_higher_or_lower_integer_range(benchmark::State& state)
{
  algorithms::guess_number_higher_or_lower::number = state.range(0);
  for (auto _ : state) {
    auto result = algorithms::guess_number_higher_or_lower::integer_range(state.range(1));
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_guess_number_higher_or_lower_integer_range)
  ->Args({ 123, std::numeric_limits<int>::max() - 1 });
// clang-format on

static void algorithms_guess_number_higher_or_lower_const_integer_range(benchmark::State& state)
{
  algorithms::guess_number_higher_or_lower::number = state.range(0);
  for (auto _ : state) {
    auto result = algorithms::guess_number_higher_or_lower::const_integer_range(state.range(1));
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_guess_number_higher_or_lower_const_integer_range)
  ->Args({ 123, std::numeric_limits<int>::max() - 1 });
// clang-format on

static void algorithms_guess_number_higher_or_lower_bidirectional(benchmark::State& state)
{
  algorithms::guess_number_higher_or_lower::number = state.range(0);
  for (auto _ : state) {
    auto result = algorithms::guess_number_higher_or_lower::bidirectional(state.range(1));
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_guess_number_higher_or_lower_bidirectional)
  ->Args({ 123, std::numeric_limits<int>::max() - 1 });
// clang-format on

static void algorithms_guess_number_higher_or_lower_const_bidirectional(benchmark::State& state)
{
  algorithms::guess_number_higher_or_lower::number = state.range(0);
  for (auto _ : state) {
    auto result = algorithms::guess_number_higher_or_lower::const_bidirectional(state.range(1));
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_guess_number_higher_or_lower_const_bidirectional)
  ->Args({ 123, std::numeric_limits<int>::max() - 1 });
// clang-format on

static void algorithms_guess_number_higher_or_lower_random_access(benchmark::State& state)
{
  algorithms::guess_number_higher_or_lower::number = state.range(0);
  for (auto _ : state) {
    auto result = algorithms::guess_number_higher_or_lower::random_access(state.range(1));
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_guess_number_higher_or_lower_random_access)
  ->Args({ 123, std::numeric_limits<int>::max() - 1 });
// clang-format on

static void algorithms_guess_number_higher_or_lower_const_random_access(benchmark::State& state)
{
  algorithms::guess_number_higher_or_lower::number = state.range(0);
  for (auto _ : state) {
    auto result = algorithms::guess_number_higher_or_lower::const_random_access(state.range(1));
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_guess_number_higher_or_lower_const_random_access)
  ->Args({ 123, std::numeric_limits<int>::max() - 1 });
// clang-format on

static void algorithms_guess_number_higher_or_lower_legacy_random_access(benchmark::State& state)
{
  algorithms::guess_number_higher_or_lower::number = state.range(0);
  for (auto _ : state) {
    auto result = algorithms::guess_number_higher_or_lower::legacy_random_access(state.range(1));
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_guess_number_higher_or_lower_legacy_random_access)
  ->Args({ 123, std::numeric_limits<int>::max() - 1 });
// clang-format on

static void algorithms_guess_number_higher_or_lower_const_legacy_random_access(benchmark::State& state)
{
  algorithms::guess_number_higher_or_lower::number = state.range(0);
  for (auto _ : state) {
    auto result = algorithms::guess_number_higher_or_lower::const_legacy_random_access(state.range(1));
    benchmark::DoNotOptimize(result);
  }
}

// clang-format off
BENCHMARK(algorithms_guess_number_higher_or_lower_const_legacy_random_access)
  ->Args({ 123, std::numeric_limits<int>::max() - 1 });
// clang-format on

#endif  // ENABLE_BENCHMARKS
