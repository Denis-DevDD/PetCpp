#include "UseCommand.hpp"
#include <iostream>
#include "Exceptions.hpp"

bool UseCommand::execute(Game *game)
{
  std::vector<std::string> input = game->getParser()->getUserInput();
  if (input.size() != 3)
  {
    throw CommandExceptions("E_INVALID_PARAM_COUNT");
  }
  if (game->getParser()->getIfThrow() == true)
  {
    throw WrongParamExceptions("E_INVALID_PARAM");
  }

  Room* current_room = game->getCurrentRoom();
  std::string id = game->getParser()->getUserInput()[1];
  if (current_room->getNeededRoom(id) == nullptr)
  {
    throw CommandExceptions("E_ENTITY_OR_ITEM_UNAVAILABLE");
  }
  Field* player_field = current_room->getNeededRoom(id);
  Player* player = (Player*)player_field->getEntity();
  std::string item_to_use = input.at(2);

  if(player->getEquippedw()!=nullptr)
  {
    if(item_to_use==player->getEquippedw()->getName())
    {
      player->addInventory(player->getEquippedw(), (1));
      player->setEquippedw(nullptr);
      return true;
    }
  }
  if(player->getEquippeda()!=nullptr)
  {
    if(item_to_use==player->getEquippeda()->getName())
    {
      player->addInventory(player->getEquippeda(), (1));
      player->setEquippeda(nullptr);
      return true;
    }
  }

  for(auto& i : player->getInventory())
  {
    if(item_to_use == i.first->getName())
    {
      if((i.first->getName()=="ARRW")||(i.first->getName()=="BOLT"))
      {
        throw WrongParamExceptions("E_INVALID_PARAM");
      }

      if((i.first->getItemType()==ItemType::CONSUMABLE))
      {
        std::cout<<player->getClassName()<<" ["<<player->getId()<<"] \""<<player->getName()<<"\" consumed \""<<i.first->getFullName()<<"\"."<<std::endl;
        i.first->usePotion(player);
        player->remLoot(i.first,1);
      }

      if((i.first->getItemType()==ItemType::WEAPON))
      {
        if(player->getEquippedw()!=nullptr)
        {
          player->addInventory(player->getEquippedw(),1);
        }
        player->setEquippedw(i.first);
      }
      if((i.first->getItemType()==ItemType::ARMOR))
      {
        if(player->getEquippeda()!=nullptr)
        {
          player->addInventory(player->getEquippeda(),1);
        }
        player->setEquippeda(i.first);
      }
      player->remInventory(i.first,1);
      return true;
    }
  }
  throw WrongParamExceptions("E_INVALID_PARAM");
}
