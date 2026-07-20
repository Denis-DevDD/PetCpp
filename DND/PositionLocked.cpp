#include "PositionLocked.hpp"

void PositionLocked::printLoot()
{
  std::vector<std::pair<Item*,int>> armor;
  std::vector<std::pair<Item*,int>> weapons;
  std::vector<std::pair<Item*,int>> consumables;

  for(auto i : loot_)
  {
    if(i.first->getItemType()==ItemType::ARMOR)
    {
      armor.push_back({i.first,i.second});
    }
  }
  for(auto i : loot_)
  {
    if(i.first->getItemType()==ItemType::WEAPON)
    {
      weapons.push_back({i.first,i.second});
    }
  }
  for(auto i : loot_)
  {
    if(i.first->getItemType()==ItemType::CONSUMABLE)
    {
      consumables.push_back({i.first,i.second});
    }
  }

  if(!armor.empty())
  {
    sort(armor.begin(),armor.end(),Functions::compareItems);
    std::cout<<"  Armor:";
    size_t iterator = 0;
    size_t end = armor.size();
    for(auto i : armor)
    {
      std::cout <<" ["<<i.first->getName()<<"] "<<i.first->getFullName()<<" ("<<i.second<<")";
      iterator++;
      if(iterator!=end)
      {
        std::cout<<",";
      }
    }
    std::cout<<std::endl;
  }
  if(!weapons.empty())
  {
    sort(weapons.begin(),weapons.end(),Functions::compareItems);
    std::cout<<"  Weapons:";
    size_t iterator = 0;
    size_t end = weapons.size();
    for(auto i : weapons)
    {
      std::cout <<" ["<<i.first->getName()<<"] "<<i.first->getFullName()<<" ("<<i.second<<")";
      iterator++;
      if(iterator!=end)
      {
        std::cout<<",";
      }
    }
    std::cout<<std::endl;
  }
  if(!consumables.empty())
  {
    sort(consumables.begin(),consumables.end(),Functions::compareItems);
    std::cout<<"  Consumables:";
    size_t iterator = 0;
    size_t end = consumables.size();
    for(auto i : consumables)
    {
      std::cout <<" ["<<i.first->getName()<<"] "<<i.first->getFullName()<<" ("<<i.second<<")";
      iterator++;
      if(iterator!=end)
      {
        std::cout<<",";
      }
    }
    std::cout<<std::endl;
  }
}
