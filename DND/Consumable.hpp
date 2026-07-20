#ifndef CONSUMABLE_HPP
#define CONSUMABLE_HPP

#include "Item.hpp"
#include <string>
#include "Player.hpp"
enum class Type
{
  POTION, AMMUNITION
};

enum class SubType
{
  NHEP, GHEP, SHEP, FIRS, CORS, FORS, ACRS, ARRW, BOLT
};

class Consumable : public Item
{
private:
  Type type_;
  SubType subtype_;
  DamageType damage_res_;
  std::string full_name_;
public:
  SubType nametoSubType(std::string name);

  void setType(SubType);

  Consumable(std::string name) : Item(name)
  {
    damage_res_ = DamageType::NONE;
    item_type_ = ItemType::CONSUMABLE;
    subtype_ = nametoSubType(name);
    full_name_ = nameToFullName(name);
    setType(subtype_);
  }

  std::string getFullName() const override{return full_name_;}

  std::string nameToFullName(std::string name);

  void usePotion(Player* player) override;

  DamageType getDmgREsistanmces() override;

   ~Consumable() override {};

  Consumable(const Consumable&) = delete;
};

#endif
