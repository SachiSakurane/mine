#include <iostream>

#include "schema/game_result.hpp"
#include "area.hpp"
#include "controller.hpp"
#include "perse_parameters.hpp"

player move_player(const player &p, const area &a, const controller_input i)
{
  player result = p;
  switch (i)
  {
  case controller_input::kUp:
    result.y = (p.y + a.height - 1) % a.height;
    break;
  case controller_input::kDown:
    result.y = (p.y + a.height + 1) % a.height;
    break;
  case controller_input::kRight:
    result.x = (p.x + a.width + 1) % a.width;
    break;
  case controller_input::kLeft:
    result.x = (p.x + a.width - 1) % a.width;
    break;
  default:
    break;
  }
  return result;
}

area make_area(const difficulty d)
{
  switch (d)
  {
  case difficulty::kEasy:
    return area{9, 9, 10};
  case difficulty::kAdvanced:
    return area{16, 16, 40};
  case difficulty::kHard:
    return area{30, 16, 99};
  case difficulty::kImpossible:
    return area{50, 50, 500};
  }
}

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

  bool game = true;
  game_result result;
  controller ctrl;
  player player;
  auto area = make_area(params.selected_difficulty);
  std::system("clear");

  std::cout << "Press any key..." << std::endl;

  ctrl.wait_input();

  while (game && ctrl.wait_input())
  {
    std::system("clear");

    player = move_player(player, area, ctrl.get_current_input());

    if (ctrl.get_current_input() == controller_input::kDig)
    {
      if (!area.dig(player))
      {
        game = false;
        result = game_result::kGameOver;
      }
    }
    else if (ctrl.get_current_input() == controller_input::kFlag)
    {
      area.flag(player);
    }

    if (game && area.is_clear())
    {
      game = false;
      result = area.is_complete() ? game_result::kComplete : game_result::kClear;
    }
    else
    {
      std::cout << "remain: " << area.width * area.height - area.bombs - area.digged_count() << std::endl;
      std::cout << area.show_map(player) << std::endl;
    }
  }

  std::system("clear");

  switch (result)
  {
  case game_result::kGameOver:
    std::cout << area.show_bomb_map(player) << std::endl;
    std::cout << "You're an idiot. Drop dead." << std::endl;
    break;
  case game_result::kClear:
    std::cout << area.show_map(player) << std::endl;
    std::cout << "It looks like you've cleared it, but it's not perfect." << std::endl;
    break;
  case game_result::kComplete:
    std::cout << area.show_map(player) << std::endl;
    std::cout << "Perfect." << std::endl;
    break;
  }

  return 0;
}
