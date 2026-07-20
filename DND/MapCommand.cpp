#include "MapCommand.hpp"
#include <iostream>
#include "Exceptions.hpp"
bool MapCommand::execute(Game* game)
{
  std::vector<std::string> input = game->getParser()->getUserInput();
  if (input.size() != 1)
  {
    throw CommandExceptions("E_INVALID_PARAM_COUNT");
  }

  if (game->getMapOutput())
  {
    game->setMapOutput(false);
  }
  else
  {
    game->setMapOutput(true);
    std::cout<<game->getCurrentRoom();
  }
  return true;

}
