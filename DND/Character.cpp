#include "Character.hpp"

Character::Character(int max_hp,int armor,int str,int vit) : Entity()
{
  max_hp_=max_hp;
  current_hp_=max_hp_;
  armor_value_=armor;
  strength_=str;
  vitality_=vit;
  alive_=true;
  if_looted_ = false;
}

void Character::kill()
{
  alive_ = false;
  if (type_id_ != "L")
  {
    type_id_ = "X";
  }
}

void Character::setId(std::string id)
{
  type_id_ = id;
}

void Character::setHp(int hp)
{
  current_hp_=hp;
}

bool Character::ifEnemy()
{
  if (type_id_=="G" || type_id_=="Z" || type_id_=="L")
  {
    return true;
  }
    else
    {
      return false;
    }
}

void Character::addInventory(Item* item, int amount)
{
  for(auto& i : inventory_)
  {
    if(item==i.first)
    {
      int new_amount = i.second + amount;
      i.second=new_amount;
      return;
    }
  }
  inventory_.insert({item,amount});
}

void Character::remInventory(Item* item, int amount)
{
  for(auto& i : inventory_)
  {
    if(item==i.first)
    {
      int new_amount = i.second - amount;
      i.second=new_amount;
      if(i.second<=0)
      {
        inventory_.erase(i.first);
      }
      return;
    }
  }
}

void Character::printLoot()
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
    sort(armor.begin(),armor.end(),compareIt);
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
    sort(weapons.begin(),weapons.end(),compareIt);
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
    sort(consumables.begin(),consumables.end(),compareIt);
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

void Character::addLoot(Item* item, int amount)
{
  for(auto& i : loot_)
  {
    if(item==i.first)
    {
      i.second=i.second+amount;
      return;
    }
  }
  loot_.insert({item,amount});
}

void Character::remLoot(Item* item, int amount)
{
  for(auto& i : loot_)
  {
    if(item==i.first)
    {
      int new_amount = i.second - amount;
      i.second=new_amount;
      if(i.second<=0)
      {
        loot_.erase(i.first);
      }
      return;
    }
  }
}

void Character::setifWasLooted(bool if_looted)
{
  if (type_id_ != "W" && type_id_ != "B" && type_id_ != "R")
  {
    if_looted_ = if_looted;
  }
}
std::string Character::getId() const
{
  return type_id_;
}
int Character::getHP() const
{
  return current_hp_;
}

int Character::getVital() const
{
  return vitality_;
}

int Character::getStr() const
{
  return strength_;
}

Item* Character::getEquippedw()
{
  return equipped_weapon_;
}

Item* Character::getEquippeda()
{
  return equipped_armor_;
}

std::map<Item*, int> Character::getInventory()
{
  return inventory_;
}

int Character::getArmorVal() const
{
  return armor_value_;
}
int Character::getMaxHP() const
{
  return max_hp_;
}

std::string Character::getName() const
{
  return "no_name";
}

std::string Character::getClassName() const
{
  return "no_name";
}

bool Character::getIfPositionLocked() const
{
  return false;
}

bool Character::getIfDead() const
{
  return alive_;
}

bool Character::compareIt(const std::pair<Item*, int>& a, const std::pair<Item*, int>& b)
{
  return a.first->getFullName() < b.first->getFullName();
}

std::map<Item*,int> Character::getLoot()
{
  return loot_;
}

void Character::setEquippeda(Item* item)
{
  equipped_armor_=item;
}

void Character::setEquippedw(Item* item)
{
  equipped_weapon_=item;
}

bool Character::getifWasLooted()
{
  return if_looted_;
}
