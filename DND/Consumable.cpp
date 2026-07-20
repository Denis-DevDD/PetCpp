



#include "Consumable.hpp"
#include <map>
#include "Random.hpp"

SubType Consumable::nametoSubType(std::string name)
{
  std::map<std::string, SubType> ctransfer = { {"NHEP", SubType::NHEP},
  {"GHEP", SubType::GHEP}, {"SHEP", SubType::SHEP}, {"FIRS", SubType::FIRS},
  {"CORS", SubType::CORS}, {"FORS", SubType::FORS}, {"ACRS", SubType::ACRS},
  {"ARRW", SubType::ARRW}, {"BOLT", SubType::BOLT} };
  auto pair = ctransfer.find(name);
  return pair->second;
}

void Consumable::setType(SubType)
{
  if ((subtype_== SubType::NHEP) || (subtype_ == SubType::GHEP) || (subtype_ == SubType::SHEP) ||
  (subtype_ == SubType::FIRS) || (subtype_ == SubType::CORS) || (subtype_ == SubType::FORS) ||
  (subtype_ == SubType::ACRS))
  {
    type_ = Type::POTION;
  }
    else {
      type_ = Type::AMMUNITION;
    }
}

std::string Consumable::nameToFullName(std::string name)
{
  std::map<std::string, std::string> ctransfer = { {"NHEP", "Normal Health Potion"},
  {"GHEP", "Greater Health Potion"}, {"SHEP", "Superior Health Potion"},
  {"FIRS", "Potion of Fire Resistance"}, {"CORS", "Potion of Cold Resistance"},
  {"FORS", "Potion of Force Resistance"}, {"ACRS", "Potion of Acid Resistance"},
  {"ARRW", "Arrow"}, {"BOLT", "Bolt"} };
  auto pair = ctransfer.find(name);
  return pair->second;
}

void Consumable::usePotion(Player* player)
{
  if((subtype_ == SubType::NHEP)||(subtype_ == SubType::GHEP)||(subtype_ == SubType::SHEP))
  {
    int regen = 0;
    if(subtype_ == SubType::NHEP)
    {
      regen = 2*Oop::Random::getInstance().getRandomNumber(4);
      std::cout<<"[Dice Roll] 2 d4 resulting in a total value of "<<regen<<"."<<std::endl;
    }
    if(subtype_ == SubType::GHEP)
    {
      regen = 4*Oop::Random::getInstance().getRandomNumber(4);
      std::cout<<"[Dice Roll] 4 d4 resulting in a total value of "<<regen<<"."<<std::endl;
    }
    if(subtype_ == SubType::SHEP)
    {
      regen = 8*Oop::Random::getInstance().getRandomNumber(4);
      std::cout<<"[Dice Roll] 8 d4 resulting in a total value of "<<regen<<"."<<std::endl;
    }
    player->setHp(player->getHP()+regen);
    if(player->getHP()>player->getMaxHP())
    {
      player->setHp(player->getMaxHP());
    }
    std::cout<<player->getClassName()<<" ["<<player->getId()<<"] \""<<player->getName()<<"\" regenerates "<<regen<<" health."<<std::endl;
  }
  else
  {
    std::string res;
    if(subtype_==SubType::FIRS)
    {
      player->setResist(DamageType::FIRE);
      res = "Fire";
    }
    if(subtype_==SubType::CORS)
    {
      player->setResist(DamageType::COLD);
      res = "Cold";
    }
    if(subtype_==SubType::FORS)
    {
      player->setResist(DamageType::FORCE);
      res = "Force";
    }
    if(subtype_==SubType::ACRS)
    {
      player->setResist(DamageType::ACID);
      res = "Acid";
    }
    std::cout<<player->getClassName()<<" ["<<player->getId()<<"] \""<<player->getName()<<" is now resistant to "<<res<<" until leaving the room"<<std::endl;
  }
}

DamageType Consumable::getDmgREsistanmces()
{
  if(subtype_==SubType::FIRS)
  {
    return DamageType::FIRE;
  }
  if(subtype_==SubType::CORS)
  {
    return DamageType::COLD;
  }
  if(subtype_==SubType::FORS)
  {
    return DamageType::FORCE;
  }
  if(subtype_==SubType::ACRS)
  {
    return DamageType::ACID;
  }
  return DamageType::NONE;
}