#pragma once

#include <vector>

#include "cell.hpp"

class area
{
public:
  area(std::size_t width, std::size_t height) : bomb_map(width, std::vector<bool>(height, false)),
                                                screen_map(width, std::vector<cell>(height, cell::kUnknown)) {}

private:
  std::vector<std::vector<bool>> bomb_map;
  std::vector<std::vector<cell>> screen_map;
};
