#include "Player.hpp"
#include <iomanip>
#include "Room.hpp"

Player::Player(std::string name, int max_hp, int armor, int str, int vit, std::string id)
: Character(max_hp+vit, armor, str, vit)
{
  setId(id);
  name_ = name;
  fleed_ = false;
  if_player_ = true;
  setHp(max_hp+vit);
  if (id == "W")
  {
    name_of_class_ = "Wizard";

    equipped_armor_ = nullptr;

    equipped_weapon_ = all_items_->find("QFRC")->second;

    loot_.insert({all_items_->find("QFRC")->second,1});

    inventory_.insert({all_items_->find("QACD")->second,1});
    loot_.insert({all_items_->find("QACD")->second,1});

    inventory_.insert({all_items_->find("DAGG")->second,1});
    loot_.insert({all_items_->find("DAGG")->second,1});
    resist_ = DamageType::NONE;
  }
  if (id == "B")
  {
    name_of_class_ = "Barbarian";

    equipped_armor_ = nullptr;

    equipped_weapon_ = all_items_->find("GAXE")->second;

    loot_.insert({all_items_->find("GAXE")->second, 1});

    inventory_.insert({all_items_->find("HAXE")->second, 2});
    loot_.insert({all_items_->find("HAXE")->second, 2});
    resist_ = DamageType::NONE;
  }
  if (id == "R")
  {
    name_of_class_ = "Rogue";

    equipped_weapon_ = all_items_->find("RAPI")->second;

    equipped_armor_ = all_items_->find("LARM")->second;

    loot_.insert({all_items_->find("RAPI")->second, 1});

    loot_.insert({all_items_->find("LARM")->second, 1});

    inventory_.insert({all_items_->find("DAGG")->second, 2});
    loot_.insert({all_items_->find("DAGG")->second, 2});

    inventory_.insert({all_items_->find("SBOW")->second, 1});
    loot_.insert({all_items_->find("SBOW")->second, 1});

    inventory_.insert({all_items_->find("ARRW")->second, 20});
    loot_.insert({all_items_->find("ARRW")->second, 20});
    resist_ = DamageType::NONE;
  }
}

Field* Player::findMySelf(Room* room)
{
  return room->getNeededRoom(type_id_);
}

bool Player::checkAmmunition(std::string name)
{

  Item* needed_item = all_items_->find(name)->second;

  for (auto& pair : inventory_)
  {
    if (pair.first == needed_item)
    {
      if (pair.second > 0)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }
  return false;
}

void Player::fromXtoId()
{
  if (name_of_class_ == "Wizard")
  {
    type_id_ = "W";
  }
  else if (name_of_class_ == "Rogue")
  {
    type_id_ = "R";
  }
  else
  {
    type_id_ = "B";
  }
}
