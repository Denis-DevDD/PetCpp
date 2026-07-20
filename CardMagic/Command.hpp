#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>
#include <string>
#include "GameError.hpp"

enum class CommandType
{
  QUIT, HELP, GRAVEYARD, BOARD, HAND, INFO, REDRAW, STATUS,
  DONE, BATTLE, CREATURE, SPELL
};

class Command
{
private:
  CommandType type_;
  std::vector<std::string> parameters_;

public:
  Command(CommandType type) : type_{type} {};
  Command(const Command&)=default;
  ~Command() = default;
  Command(std::vector<std::string>& input);
  bool isQuit();
  CommandType getType() const;
  const std::vector<std::string>& getParameters() const;
  void setType(CommandType type);
};

#endif
