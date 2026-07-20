#include "LootCommand.hpp"
#include <iostream>
#include "Exceptions.hpp"
#include "Random.hpp"

bool LootCommand::execute(Game* game)
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

  Room* current_room = game->getCurrentRoom();
  int x = game->getParser()->getCrd()[1]-1;
  int y = game->getParser()->getCrd()[0]-1;

  current_room->checkRange(y+1, x);
  current_room->checkRange(y, x);
  Field* loot_field = current_room->getFields()[y][x];

  std::string id = game->getParser()->getUserInput()[1];
  if (current_room->getNeededRoom(id) == nullptr)
  {
    throw CommandExceptions("E_ENTITY_OR_ITEM_UNAVAILABLE");
  }
  Field* player_field = current_room->getNeededRoom(id);

  current_room->canLoot(player_field, y, x);

  if(loot_field->getEntity()!=nullptr)
  {

    if(loot_field->getEntity()->getId() == "T")
    {
      size_t min_value_to_roll = loot_field->getEntity()->getDiceToBeat() - player_field->getEntity()->getVital();
      std::cout<<"** To unlock this chest you need to roll at least "<<min_value_to_roll<<" to reach the "<<loot_field->getEntity()->getDiceToBeat()<<" needed."<<std::endl;

      size_t roll = Oop::Random::getInstance().getRandomNumber(20);
      std::cout<<"[Dice Roll] 1 d20 resulting in a total value of "<<roll<<"."<<std::endl<<std::endl;
      if(roll>=min_value_to_roll)
      {
        loot_field->getEntity()->setLock(false);
      }
      if(!loot_field->getEntity()->getIfLocked())
      {
        std::string openmsg = game->getStoryMessage("N_LOOT_SUCCESSFUL");
        std::cout<<openmsg;
        loot_field->getEntity()->printLoot();
        for(auto item : loot_field->getEntity()->getLoot())
        {
          player_field->getEntity()->addInventory(item.first,item.second);
          player_field->getEntity()->addLoot(item.first,item.second);
          loot_field->getEntity()->eraseLoot(item.first);
        }
        if (!loot_field->getEntity()->getIfplayer())
        {
        entities_to_deallocate.push_back(loot_field->getEntity());
        }
        loot_field->getEntity()->setifWasLooted(true);
        loot_field->setEntity(nullptr);
      }
      else
      {
        std::string lockmsg = game->getStoryMessage("N_LOOT_CHEST_LOCKED");
        std::cout<<lockmsg;
      }
    }
    else if(loot_field->getEntity()->getId()=="X")
    {
      std::string openmsg = game->getStoryMessage("N_LOOT_SUCCESSFUL");
      std::cout<<openmsg;
      loot_field->getEntity()->printLoot();
      for(auto item : loot_field->getEntity()->getLoot())
      {
        player_field->getEntity()->addInventory(item.first,item.second);
        player_field->getEntity()->addLoot(item.first,item.second);
        loot_field->getEntity()->eraseLoot(item.first);
      }
      if (!loot_field->getEntity()->getIfplayer())
      {
        entities_to_deallocate.push_back(loot_field->getEntity());
      }
      loot_field->getEntity()->setifWasLooted(true);
      loot_field->setEntity(nullptr);
    }
  }
  return true;
}

LootCommand::~LootCommand()
{
  for (auto& ent : entities_to_deallocate)
  {
    if (ent!=nullptr)
    {
      delete ent;
    }
  }
}
