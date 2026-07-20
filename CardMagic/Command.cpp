#include <iostream>
#include <map>
#include "Command.hpp"

Command::Command(std::vector<std::string>& input)
{
  std::map<std::string, CommandType> commands =
  {
    {"QUIT", CommandType::QUIT},
    {"HELP", CommandType::HELP},
    {"GRAVEYARD", CommandType::GRAVEYARD},
    {"BOARD", CommandType::BOARD},
    {"HAND", CommandType::HAND},
    {"INFO", CommandType::INFO},
    {"REDRAW", CommandType::REDRAW},
    {"STATUS", CommandType::STATUS},
    {"DONE", CommandType::DONE},
    {"BATTLE", CommandType::BATTLE},
    {"CREATURE", CommandType::CREATURE},
    {"SPELL", CommandType::SPELL}
  };
  auto type = commands.find(input[0]);
  if (type != commands.end())
  {
    type_ = type->second;
  }
  else
  {
    throw UnknownCommandException();
  }

  parameters_.insert(parameters_.end(), input.begin() + 1, input.end());
}

bool Command::isQuit()
{
  if (type_ == CommandType::QUIT)
  {
    return true;
  }
  return false;
}

CommandType Command::getType() const
{
  return type_;
}

const std::vector<std::string>& Command::getParameters() const
{
  return parameters_;
}

void Command::setType(CommandType type)
{
  type_ = type;
}
