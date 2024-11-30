#include "area.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <sstream>

std::size_t bomb_num(std::size_t x, std::size_t y, const area::map_type &bombs)
{
  std::size_t num = 0;
  num += x > 0 && y > 0 ? !bombs[y - 1][x - 1] : 0;
  num += x > 0 ? !bombs[y][x - 1] : 0;
  num += x > 0 && y + 1 < bombs.size() ? !bombs[y + 1][x - 1] : 0;
  num += y > 0 ? !bombs[y - 1][x] : 0;
  num += y + 1 < bombs.size() ? !bombs[y + 1][x] : 0;
  num += x + 1 < bombs[y].size() && y > 0 ? !bombs[y - 1][x + 1] : 0;
  num += x + 1 < bombs[y].size() ? !bombs[y][x + 1] : 0;
  num += x + 1 < bombs[y].size() && y + 1 < bombs.size() ? !bombs[y + 1][x + 1] : 0;

  return num;
}

char cell_string(std::size_t x, std::size_t y, cell c, const area::map_type &bombs)
{
  if (c == cell::kDigged)
  {
    char num = '0' + bomb_num(x, y, bombs);
    return num == '0' ? static_cast<char>(c) : num;
  }
  return static_cast<char>(c);
}

char bomb_string(std::size_t x, std::size_t y, const area::map_type &bombs)
{
  return bombs[y][x] ? '-' : '*';
}

bool area::dig(const player &p)
{
  // もし bomb_map が empty なら initialize
  if (bomb_map.empty())
  {
    inisialize_bomb(p);
  }

  if (dig_map[p.y][p.x] == false)
  {
    dig_map[p.y][p.x] = true;
    ++digged;
  }

  if (bomb_num(p.x, p.y, bomb_map) == 0)
  {
    if (p.x > 0 && p.y > 0 && !dig_map[p.y - 1][p.x - 1])
      dig(player{p.x - 1, p.y - 1});
    if (p.x > 0 && !dig_map[p.y][p.x - 1])
      dig(player{p.x - 1, p.y});
    if (p.x > 0 && p.y + 1 < dig_map.size() && !dig_map[p.y + 1][p.x - 1])
      dig(player{p.x - 1, p.y + 1});
    if (p.y > 0 && !dig_map[p.y - 1][p.x])
      dig(player{p.x, p.y - 1});
    if (p.y + 1 < dig_map.size() && !dig_map[p.y + 1][p.x])
      dig(player{p.x, p.y + 1});
    if (p.x + 1 < dig_map[p.y].size() && p.y > 0 && !dig_map[p.y - 1][p.x + 1])
      dig(player{p.x + 1, p.y - 1});
    if (p.x + 1 < dig_map[p.y].size() && !dig_map[p.y][p.x + 1])
      dig(player{p.x + 1, p.y});
    if (p.x + 1 < dig_map[p.y].size() && p.y + 1 < dig_map.size() && !dig_map[p.y + 1][p.x + 1])
      dig(player{p.x + 1, p.y + 1});
  }

  return bomb_map[p.y][p.x];
}

void area::flag(const player &p)
{
  flag_map[p.y][p.x] = !flag_map[p.y][p.x];
}

bool area::is_clear() const
{
  return digged + bombs == width * height;
}

bool area::is_complete() const
{
  for (std::size_t y = 0; y < flag_map.size(); ++y)
  {
    for (std::size_t x = 0; x < flag_map[y].size(); ++x)
    {
      if (flag_map[y][x] != bomb_map[y][x])
      {
        return false;
      }
    }
  }
  return true;
}

std::string area::show_map(const player &p) const
{
  const auto select_cell = [](auto x, auto y, const auto &dig, const auto &flag)
  {
    return flag[y][x] ? cell::kFlag : (dig[y][x] ? cell::kDigged : cell::kUnknown);
  };

  std::stringstream ss;

  for (std::size_t y = 0; y < dig_map.size(); ++y)
  {
    ss << std::endl;
    for (std::size_t x = 0; x < dig_map[y].size(); ++x)
    {
      if (p.x == x && p.y == y)
      {
        ss << "[" << cell_string(x, y, select_cell(x, y, dig_map, flag_map), bomb_map) << "]";
      }
      else if (p.x + 1 == x && p.y == y)
      {
        ss << cell_string(x, y, select_cell(x, y, dig_map, flag_map), bomb_map);
      }
      else
      {
        ss << " " << cell_string(x, y, select_cell(x, y, dig_map, flag_map), bomb_map);
      }
    }
  }

  return ss.str();
}

std::string area::show_bomb_map(const player &p) const
{
  std::stringstream ss;

  for (std::size_t y = 0; y < bomb_map.size(); ++y)
  {
    ss << std::endl;
    for (std::size_t x = 0; x < bomb_map[y].size(); ++x)
    {
      if (p.x == x && p.y == y)
      {
        ss << "[" << bomb_string(x, y, bomb_map) << "]";
      }
      else if (p.x + 1 == x && p.y == y)
      {
        ss << bomb_string(x, y, bomb_map);
      }
      else
      {
        ss << " " << bomb_string(x, y, bomb_map);
      }
    }
  }

  return ss.str();
}

void area::inisialize_bomb(const player &p)
{
  std::vector<std::size_t> flat_bomb_map(0);
  {
    std::vector<std::size_t> sequence(width * height);
    std::iota(sequence.begin(), sequence.end(), 0);

    // プレイヤーの位置に爆弾を置かない
    sequence.erase(std::begin(sequence) + p.x + p.y * width);

    std::random_device rd;
    std::mt19937 gen(rd());

    for (std::size_t b = 0; b < bombs; ++b)
    {
      std::uniform_int_distribution<std::size_t> dist(0, sequence.size() - 1);
      std::size_t pick = dist(gen);
      flat_bomb_map.push_back(sequence[pick]);
      sequence.erase(std::begin(sequence) + pick);
    }
  }

  bomb_map.clear();

  for (std::size_t y = 0; y < height; ++y)
  {
    std::vector<bool> v;
    for (std::size_t x = 0; x < width; ++x)
    {
      v.emplace_back(std::find(std::begin(flat_bomb_map), std::end(flat_bomb_map), x + y * width) == std::end(flat_bomb_map));
    }
    bomb_map.emplace_back(std::move(v));
  }
}