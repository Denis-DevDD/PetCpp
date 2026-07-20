#include "MoveCommand.hpp"
#include <iostream>
#include "Exceptions.hpp"
bool MoveCommand::execute(Game* game)
{
  std::vector<std::string> input = game->getParser()->getUserInput();
  if (input.size() != 3)
  {
    throw CommandExceptions("E_INVALID_PARAM_COUNT");
  }
  if (input[2].find_first_of(',') == std::string::npos)
  {
    throw WrongParamExceptions("E_INVALID_PARAM");
  }

  if (game->getParser()->getIfThrow() == true)
  {
    throw WrongParamExceptions("E_INVALID_PARAM");
  }
  Dungeon* dungeon = game->getDungeon();
  Room* current_room = game->getCurrentRoom();
  int x = game->getParser()->getCrd()[1]-1;
  int y = game->getParser()->getCrd()[0]-1;

  current_room->checkRange(y+1, x);
  current_room->checkRange(y, x);

  Field* field_to = current_room->getFields()[y][x];
  std::string id = game->getParser()->getUserInput()[1];

  if (current_room->getNeededRoom(id) == nullptr)
  {
     throw CommandExceptions("E_ENTITY_OR_ITEM_UNAVAILABLE");
  }
  Field* field_from = current_room->getNeededRoom(id);
  Entity* ent = field_from->getEntity();
  current_room->canMoveTO(field_from, y, x);

  if (field_to->getEntity() != nullptr)
  {
    if (field_to->getEntity()->getId() == "D")
    {
      if (field_to->getEntity()->getIfLocked() == true)
      {
        throw CommandExceptions("E_MOVE_LOCKED_DOOR");
      }
      if (field_to->getEntity()->getNumber()-1 < 0)
      {
        field_from->getEntity()->setIfFleed(true);
        field_from->setEntity(nullptr);
        game->setLeftPlayers(+1);
        print(x, y, ent);
        return true;
      }
      print(x, y, ent);
      game->setCurrentRoom(dungeon->getRooms()[field_to->getEntity()->getNumber()-1], ent);
      field_from->setEntity(nullptr);
      return true;
      } else
      {
        throw CommandExceptions("E_ENTITY_OR_ITEM_UNAVAILABLE");
      }
  }
  else
  {
    field_to->setEntity(field_from->getEntity());
    field_from->setEntity(nullptr);
  }
  print(x, y, ent);
  return true;

}

void MoveCommand::print(int x_s, int y_s, Entity* moved)
{

std::cout<<moved->getClassName()<<" ["<<moved->getId()<<"] \""
<<moved->getName()<<"\" "
<<"moved to ("<<y_s+1<<","<<x_s+1<<")."<<std::endl;
}
