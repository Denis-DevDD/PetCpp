#ifndef DISPLAYPLAYER_HPP
#define DISPLAYPLAYER_HPP

#include "Command.hpp"
#include <iostream>
#include "Game.hpp"
class Game;
class DisplayPlayer : public Command
{
  public:
  DisplayPlayer() : Command(false) {};
  bool execute(Game* game) override;
  ~DisplayPlayer() override {};

  DisplayPlayer(const DisplayPlayer&) = delete;

  friend class Entity;
  friend class player;
};
#endif
