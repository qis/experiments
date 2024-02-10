#include <iostream>

namespace algorithms::example {

void test()
{
  std::cout << "test" << std::endl;
}

}  // namespace algorithms::example

void test()
{
  algorithms::example::test();
}
