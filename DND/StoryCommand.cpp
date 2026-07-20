#include "StoryCommand.hpp"
#include <iostream>
#include "Exceptions.hpp"
bool StoryCommand::execute(Game* game)
{
  std::vector<std::string> input = game->getParser()->getUserInput();
  if (input.size() != 1)
  {
    throw CommandExceptions("E_INVALID_PARAM_COUNT");
  }

  if (game->getStoryOutput())
  {
    game->setStoryOutput(false);
  }
  else
  {
    game->setStoryOutput(true);
  }
  return true;
}
