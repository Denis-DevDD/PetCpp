#include "HelpCommand.hpp"
#include <iostream>
#include "Exceptions.hpp"
bool HelpCommand::execute(Game* game)
{
  std::vector<std::string> input = game->getParser()->getUserInput();
  if (input.size() > 1)
  {
    throw CommandExceptions("E_INVALID_PARAM_COUNT");
  }
  std::cout << "-- Commands ----------------------------------------\n"
  << "- help\n"
  << "     Prints this help text.\n\n"
  << "- quit\n"
  << "    Terminates the game.\n\n"
  << "- story\n"
  << "    Activates or deactivates the stories of the rooms.\n\n"
  << "- map\n"
  << "    Activates or deactivates the map.\n\n"
  << "- positions\n"
  << "    Prints the current positions of the players and enemies.\n\n"
  << "- player <PLAYER_TYPE_ABBREVIATION>\n"
  << "    Prints player information of the specific player.\n"
  << "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n\n"
  << "- inventory <PLAYER_TYPE_ABBREVIATION>\n"
  << "    Prints the inventory of the specific player.\n"
  << "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n\n"
  << "* move <PLAYER_TYPE_ABBREVIATION> <POSITION>\n"
  << "    Lets the player move to an adjacent field.\n"
  << "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
  << "    <POSITION>: field at position: <ROW>,<COLUMN>\n\n"
  << "* loot <PLAYER_TYPE_ABBREVIATION> <POSITION>\n"
  << "    Lets the player loot an adjacent chest.\n"
  << "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
  << "    <POSITION>: field at position: <ROW>,<COLUMN>\n\n"
  << "* use <PLAYER_TYPE_ABBREVIATION> <ITEM_ABBREVIATION>\n"
  << "    Lets the player use a potion or equip armor or weapons.\n"
  << "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
  << "    <ITEM_ABBREVIATION>: abbreviation of an item in the players inventory\n\n"
  << "* attack <PLAYER_TYPE_ABBREVIATION> <POSITION>\n"
  << "    Lets the player attack with the equipped weapon on the given position.\n"
  << "    <PLAYER_TYPE_ABBREVIATION>: type of the player\n"
  << "    <POSITION>: field at position: <ROW>,<COLUMN>\n\n"
  << "-- Legend ------------------------------------------\n"
  << "- display commands\n"
  << "* action commands (count as actions)\n"
  << "----------------------------------------------------\n";
  return true;
  }
