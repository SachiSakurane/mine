#pragma once

enum class cell: char {
  kUnknown = 'w',
  kBomb = '*',
  kFlag = 'F',
  kDigged = '-',
};
