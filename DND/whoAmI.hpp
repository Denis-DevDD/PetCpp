#ifndef WHOAMI_HPP
#define WHOAMI_HPP

#include "Command.hpp"
#include <iostream>
#include "Game.hpp"
#include "Exceptions.hpp"

class Game;

class WhoAmI : public Command
{
public:
  WhoAmI() : Command(false){};

  bool execute(Game* game) override
  {
    std::vector<std::string> input = game->getParser()->getUserInput();
    if (input.size() != 1)
    {
    throw CommandExceptions("E_INVALID_PARAM_COUNT");
    }
    game->getCurrentRoom();
    return true;
  };

  WhoAmI(const WhoAmI&) = delete;

  ~WhoAmI() override {};
};


#endif