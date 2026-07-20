#include "Armor.hpp"
#include <map>

Armortype Armor::nametoArmtype(std::string& name)
{
  std::map<std::string, Armortype> typenames = { {"LARM", Armortype::LARM},
  {"BPLT", Armortype::BPLT},
  {"CHML", Armortype::CHML},
  {"PARM", Armortype::PARM},
  };
  auto pair = typenames.find(name);
  return pair->second;
}

std::string Armor::nameToFullName(std::string name)
{
  std::map<std::string, std::string> typenames = { {"LARM", "Leather Armor"},
  {"BPLT", "Breastplate"},
  {"CHML", "Chain Mail"},
  {"PARM", "Plate Armor"},
  };
  auto pair = typenames.find(name);
  return pair->second;
}

Armor::Armor(std::string name) : Item(name)
{
  armtype=nametoArmtype(name);
  switch (armtype)
  {
    case Armortype::LARM :
    armor_value_ = 1;
    break;
    case Armortype::BPLT :
    armor_value_ = 4;
    break;
    case Armortype::CHML :
    armor_value_ = 6;
    break;
    case Armortype::PARM :
    armor_value_ = 8;
    break;
  }
  full_name_ = nameToFullName(name);
  item_type_ = ItemType::ARMOR;
}

void Armor::setArmValue(Entity* Char)
{
  switch (armtype)
  {
   case Armortype::LARM:
     armor_value_=1 + Char->getVital();
     break;
    case Armortype::BPLT:
     armor_value_=4 + std::min(Char->getVital(),2);
     break;
    case Armortype::CHML:
      armor_value_=6;
      break;
    case Armortype::PARM:
      armor_value_=8;
      break;
  }
}

int Armor::getArmValue() const
{
  return armor_value_;
}

std::string Armor::getFullName() const
{
  return full_name_;
}
Armortype Armor::getArmName()
{
  return armtype;
}

std::string Armor::aNameToName(Armortype armtype)
{
  std::map<Armortype, std::string> reverse_typenames = {
    {Armortype::LARM, "LARM"}, {Armortype::BPLT, "BPLT"},
    {Armortype::CHML, "CHML"}, {Armortype::PARM, "PARM"}
  };
  auto pair = reverse_typenames.find(armtype);
  return pair->second;
}