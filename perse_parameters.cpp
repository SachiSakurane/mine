#include "perse_parameters.hpp"

#include <sstream>

#include <boost/program_options.hpp>

template <class DescType>
std::string print_description(const DescType &desc)
{
  std::stringstream ss;
  ss << "usege:" << std::endl;
  ss << "  ./mine -d easy" << std::endl;
  ss << desc << std::endl;
  return ss.str();
}

command_parameters perse_parameters(const int ac, const char *const *const av)
{
  namespace po = boost::program_options;

  po::options_description desc("options");

  desc.add_options()(
      "difficulty,d", po::value<std::string>()->default_value("easy")->value_name("difficulty"),
      "difficulty. [easy, advanced, hard, impossible] available.")("help,h", "Prints help for commands");

  po::options_description perser;
  perser.add(desc);

  bool is_help = true;
  po::variables_map vm;
  command_parameters params;
  try
  {
    po::store(po::command_line_parser{ac, av}.options(perser).run(), vm);
    po::notify(vm);

    is_help = vm.count("help");
    std::string difficulty_str = vm["difficulty"].as<std::string>();

    if (difficulty_str == "easy")
    {
      params.selected_difficulty = difficulty::kEasy;
    }
    else if (difficulty_str == "advanced")
    {
      params.selected_difficulty = difficulty::kAdvanced;
    }
    else if (difficulty_str == "hard")
    {
      params.selected_difficulty = difficulty::kHard;
    }
    else if (difficulty_str == "impossible")
    {
      params.selected_difficulty = difficulty::kImpossible;
    }
    else
    {
      is_help = true;
    }
  }
  catch (po::error &e)
  {
    params.error = e.what();
    is_help = true;
  }

  if (is_help)
  {
    params.help_message = print_description(perser);
  }

  return params;
}