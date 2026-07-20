#ifndef MAPCOMMAND_HPP
#define MAPCOMMAND_HPP
#include "Command.hpp"
#include <iostream>
#include "Game.hpp"
class Game;
class MapCommand : public Command
{
  public:
  MapCommand() : Command(false) {};

  bool execute(Game* game) override;

  MapCommand(const MapCommand&) = delete;

  ~MapCommand() override {};
};
#endif
