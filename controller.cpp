#include <termios.h>
#include <unistd.h>

#include "controller.hpp"

using namespace std::string_literals;

std::string get_input_string(controller_input input)
{
  switch (input)
  {
  case controller_input::kUp:
    return "Up"s;
  case controller_input::kDown:
    return "Down"s;
  case controller_input::kRight:
    return "Right"s;
  case controller_input::kLeft:
    return "Left"s;
  case controller_input::kDig:
    return "Dig"s;
  case controller_input::kFlag:
    return "Flag"s;
  case controller_input::kNone:
    return "None"s;
  }
}

bool controller::wait_input()
{
  struct termios oldt, newt;
  char ch;
  tcgetattr(STDIN_FILENO, &oldt); // 現在の端末設定を取得
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);        // カノニカルモードとエコーを無効化
  tcsetattr(STDIN_FILENO, TCSANOW, &newt); // 新しい設定を反映
  read(STDIN_FILENO, &ch, 1);              // 1文字取得
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 元の設定を復元

  raw_input = ch;
  current_input = controller_input::kNone;

  switch (raw_input)
  {
  case 'w':
  case 'A':
    current_input = controller_input::kUp;
    break;
  case 'a':
  case 'D':
    current_input = controller_input::kLeft;
    break;
  case 's':
  case 'B':
    current_input = controller_input::kDown;
    break;
  case 'd':
  case 'C':
    current_input = controller_input::kRight;
    break;
  case ' ':
  case '\n':
    current_input = controller_input::kDig;
    break;
  case 'f':
  case '_':
    current_input = controller_input::kFlag;
    break;
  }

  return true;
}
