#pragma once

#include <cassert>
#include <string>
#include <vector>

#include "schema/cell.hpp"
#include "schema/player.hpp"

class area
{
public:
  using map_type = std::vector<std::vector<bool>>;

  area(std::size_t w, std::size_t h, std::size_t b) : width{w}, height{h}, bombs{b},
                                                      dig_map(height, std::vector<bool>(width, false)),
                                                      flag_map(height, std::vector<bool>(width, false))
  {
    assert(w * h > b);
  }

  // bomb の位置に kDigged すると false
  bool dig(const player &p);
  void flag(const player &p);
  bool is_clear() const;
  bool is_complete() const;
  std::string show_map(const player &p) const;
  std::string show_bomb_map(const player &p) const;

  const std::size_t width, height, bombs;

  std::size_t digged_count() const { return digged; }

private:
  std::size_t digged = 0;
  map_type bomb_map;
  map_type dig_map;
  map_type flag_map;

  void inisialize_bomb(const player &p);
};
