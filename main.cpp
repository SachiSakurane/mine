#include <iostream>

#include "controller.hpp"
#include "perse_parameters.hpp"

int main(const int ac, const char *const *const av)
{
  decltype(auto) params = perse_parameters(ac, av);

  if (params.help_message)
  {
    std::cout << params.help_message.value() << std::endl;
  }

  if (params.error)
  {
    std::cout << "error: " << params.error.value() << std::endl;
    return -1;
  }

  controller c;
  std::system("clear");

  while (c.wait_input())
  {
    std::system("clear");

    std::cout << c.get_raw_input() << "," << get_input_string(c.get_current_input()) << std::endl;
  }

  return 0;
}
