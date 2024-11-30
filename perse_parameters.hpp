#pragma once

#include <optional>
#include <string>
#include <vector>

#include "schema/difficulty.hpp"

struct command_parameters {
  std::optional<std::string> error;
  std::optional<std::string> help_message;
  difficulty selected_difficulty;
};

command_parameters perse_parameters(const int ac, const char *const *const av);
