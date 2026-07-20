#include "InventoryCommand.hpp"
#include <iostream>
#include "Exceptions.hpp"
#include <map>
#include <algorithm>
#include "Functions.hpp"
bool InventoryCommand::execute(Game* game)
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
  SafeAllNecessary(game);
  std::cout<<"Inventory \""<<player_->getName()<<"\""<<std::endl;

  std::cout<<"  Equipped Armor: [";
  if (equipped_armor_ == nullptr)
  {
    std::cout<<"-] None"<<std::endl;
  }
  else
  {
    std::cout<<equipped_armor_->getName()<<"] "
    <<equipped_armor_->getFullName()<<std::endl;
  }


  std::cout<<"  Equipped Weapon: [";
  if (equipped_weapon_ == nullptr)
  {
    std::cout<<"-] None"<<std::endl;
  }
  else
  {
    std::cout<<equipped_weapon_->getName()<<"] "
    <<equipped_weapon_->getFullName()<<std::endl;
  }

  printInv();
  consumes_.clear();
  weapons_.clear();
  armors_.clear();
  return true;
}

void InventoryCommand::SafeAllNecessary(Game* game)
{
  current_room_ = game->getCurrentRoom();
  id_ = game->getParser()->getUserInput()[1];
  player_ = game->getPlayer(id_);
  inv_ = player_->getInventory();
  equipped_armor_ = player_->getEquippeda();
  equipped_weapon_ = player_->getEquippedw();



  for (auto& pair : inv_)
  {
    if ((pair.first != equipped_armor_ || pair.second > 0 )
    && pair.second > 0 && pair.first->getItemType() == ItemType::ARMOR)
    {
      armors_.push_back({pair.first, pair.second});
    }
  }

  for (auto& pair : inv_)
  {
    if ((pair.first != equipped_weapon_ || pair.second > 0)
     && pair.second > 0 && pair.first->getItemType() == ItemType::WEAPON)
   {
      weapons_.push_back({pair.first, pair.second});
    }
  }

  for (auto& pair : inv_)
  {
    if (pair.second > 0 && pair.first->getItemType() == ItemType::CONSUMABLE)
    {
     consumes_.push_back({pair.first, pair.second});
    }
  }

}

void InventoryCommand::printInv()
{
  if (!armors_.empty())
  {
    std::vector<std::pair<Item*, int>> armors = armors_;
    sort(armors.begin(), armors.end(), Functions::compareItems);
    std::cout<<"  Armor:";
    for (size_t i = 0; i < armors.size(); i++)
    {
      Item* item = armors[i].first;
      int amount = armors[i].second;
      std::cout << " [" << item->getName() << "] " << item->getFullName() << " (" << amount << ")";
      if (i != armors.size()-1)
      {
        std::cout<<",";
      }
    }
    std::cout<<std::endl;
  }
  if (!weapons_.empty())
  {
    sort(weapons_.begin(), weapons_.end(), Functions::compareItems);
    std::cout<<"  Weapons:";
    for (size_t i = 0; i < weapons_.size(); i++)
    {
      Item* item = weapons_[i].first;
      int amount = weapons_[i].second;
      std::cout << " [" << item->getName() << "] " << item->getFullName() << " (" << amount << ")";
      if (i != weapons_.size()-1)
      {
        std::cout<<",";
      }
    }
    std::cout<<std::endl;
  }
  if (!consumes_.empty())
  {
    sort(consumes_.begin(), consumes_.end(), Functions::compareItems);
    std::cout<<"  Consumables:";
    for (size_t i = 0; i < consumes_.size(); i++)
    {
      Item* item = consumes_[i].first;
      int amount = consumes_[i].second;
      std::cout << " [" << item->getName() << "] " << item->getFullName() << " (" << amount << ")";
      if (i != consumes_.size()-1)
      {
        std::cout<<",";
      }
    }
    std::cout<<std::endl;
  }
}
