#include "Command.hpp"
#include <iostream>
#include "Game.hpp"
#ifndef HELPCOMMAND_HPP
#define HELPCOMMAND_HPP
class Game;
class HelpCommand : public Command
{
  public:
  HelpCommand() : Command(false) {};

  bool execute(Game* game) override;

  HelpCommand(const HelpCommand&) = delete;

  ~HelpCommand() override {};
};
#endif
