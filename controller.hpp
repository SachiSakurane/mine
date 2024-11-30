#pragma once

#include <string>

enum class controller_input
{
  kUp,
  kDown,
  kLeft,
  kRight,
  kDig,
  kFlag,
  kNone,
};

std::string get_input_string(controller_input input);

class controller
{
public:
  bool wait_input();
  char get_raw_input() const { return raw_input; }
  controller_input get_current_input() const { return current_input; }

private:
  char raw_input = 0;
  controller_input current_input = controller_input::kNone;
};
