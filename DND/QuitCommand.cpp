#include "Game.hpp"
#include "QuitCommand.hpp"
#include "Exceptions.hpp"

bool QuitCommand::execute(Game* game)
{
  std::vector<std::string> input = game->getParser()->getUserInput();
  if (input.size() > 1)
  {
    throw CommandExceptions("E_INVALID_PARAM_COUNT");
  }
  game->setPhase(GamePhase::QUIT);
  return true;
}
