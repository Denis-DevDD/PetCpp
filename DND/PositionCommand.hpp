#ifndef POSITIONCOMMAND_HPP
#define POSITIONCOMMAND_HPP
#include "Command.hpp"
#include <iostream>
#include "Game.hpp"
class Game;
class PositionCommand : public Command
{
  public:
  PositionCommand() : Command(false) {};

  bool execute(Game* game) override;

  PositionCommand(const PositionCommand&) = delete;

  ~PositionCommand() override {};
};
#endif
