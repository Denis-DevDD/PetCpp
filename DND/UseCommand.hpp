#ifndef PROJECT_0_USECOMMAND_HPP
#define PROJECT_0_USECOMMAND_HPP

#include "Command.hpp"
#include <iostream>
#include "Game.hpp"

class Game;

class UseCommand : public Command
{
public:
  UseCommand() : Command(true){};

  bool execute(Game* game) override;

  UseCommand(const UseCommand&) = delete;

  ~UseCommand() override {};
};


#endif
