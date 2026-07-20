#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "Game.hpp"
#include "GameError.hpp"
#include "Messages.hpp"
#include "CommandLine.hpp"
#include "Command.hpp"

enum ReturnValues
{
  SUCCESSFUL = 0,
  INVALID_ALLOCATION = 1,
  WRONG_NUMBER_OF_PARAMETERS = 2,
  INVALID_CONFIG_FILE = 3
};

int main(int argc, char** argv)
{
  if (argc != 3)
  {
    std::cout << "[ERROR] Wrong number of parameters.\n";
    return WRONG_NUMBER_OF_PARAMETERS;
  }

  Game game;
  int parsing_status = game.loadConfigFile(argv[1]);
  if (parsing_status == 0)
  {
    std::cout << "[ERROR] Invalid file (" << argv[1] << ").\n";
    return INVALID_CONFIG_FILE;
  }
  else if (parsing_status == 1)
  {
    std::cout << "[ERROR] Not enough memory!\n";
    return INVALID_ALLOCATION;
  }

  if(!Messages::loadMessages(argv[2]))
  {
    std::cout << "[ERROR] Invalid file (" << argv[2] << ").\n";
    return INVALID_CONFIG_FILE;
  }

  game.start();

  if (!game.makeGame())
  {
    return INVALID_ALLOCATION;
  }

  return SUCCESSFUL;
}
