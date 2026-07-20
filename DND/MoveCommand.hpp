#ifndef MOVECOMMAND_HPP
#define MOVECOMMAND_HPP
#include "Command.hpp"
#include <iostream>
#include "Game.hpp"
class Game;
class MoveCommand : public Command
{
  public:
  MoveCommand() : Command(true) {};

  bool execute(Game* game) override;

  void print(int x_s, int y_s, Entity* moved);

  MoveCommand(const MoveCommand&) = delete;

  ~MoveCommand() override {};
};
#endif
