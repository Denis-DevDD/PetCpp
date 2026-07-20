#include "DisplayPlayer.hpp"
#include <iostream>
#include <iomanip>
#include "Exceptions.hpp"
bool DisplayPlayer::execute(Game* game)
{
  std::vector<std::string> input = game->getParser()->getUserInput();
  if (input.size() != 2)
  {
    throw CommandExceptions("E_INVALID_PARAM_COUNT");
  }
  Room* current_room = game->getCurrentRoom();
  std::string id = game->getParser()->getUserInput()[1];
  if (current_room->getNeededRoom(id) == nullptr)
  {
     throw CommandExceptions("E_ENTITY_OR_ITEM_UNAVAILABLE");
  }
  Entity* player = current_room->getNeededRoom(id)->getEntity();
  Field* field_of_player = current_room->getNeededRoom(id);

  std::cout<<player->getClassName()<<" ["<<player->getId()<<"] \""
  <<player->getName()<<"\" on ("<<field_of_player->getY()+1<<","<<field_of_player->getX()+1<<")"<<std::endl;

  int armor_output = 0;
  if (player->getEquippeda() != nullptr)
  {
    if (player->getEquippeda()->getArmValue() >= player->getArmorVal())
    {
      player->getEquippeda()->setArmValue(player);
      armor_output = player->getEquippeda()->getArmValue();
    }
    else
    {
      armor_output = player->getArmorVal();
    }
  }
  else
  {
    armor_output = player->getArmorVal();
  }


  std::cout<<"  Armor Value:    "<<std::setw(5)<<armor_output<<std::endl;
  std::cout<<"  Current Health: "<<std::setw(5)<<player->getHP()<<std::endl;
  std::cout<<"  Max Health:     "<<std::setw(5)<<player->getMaxHP()<<std::endl;
  std::cout<<"  Strength:       "<<std::setw(5)<<player->getStr()<<std::endl;
  std::cout<<"  Vitality:       "<<std::setw(5)<<player->getVital()<<std::endl;
  return true;
}
