#ifndef STORYCOMMAND_HPP
#define STORYCOMMAND_HPP
#include "Command.hpp"
#include <iostream>
#include "Game.hpp"
class Game;
class StoryCommand : public Command
{
  public:
  StoryCommand() : Command(false) {};

  bool execute(Game* game) override;

  StoryCommand(const StoryCommand&) = delete;

  ~StoryCommand() override {};
};
#endif
