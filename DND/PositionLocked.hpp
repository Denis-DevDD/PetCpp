#ifndef POSITIONLOCKED_HPP
#define POSITIONLOCKED_HPP

#include "Entity.hpp"
#include <string>
#include <map>
#include <algorithm>
#include "Functions.hpp"

class Room;

class PositionLocked : public Entity
{
private:
  std::string type_;
  bool locked_;
  int dice_to_beat_;
  int leads_to_room_id_;
  Room *leads_to_room_ptr_;
  std::map<Item*, int> loot_;
public:
  PositionLocked(std::string type, bool locked, int dice_to_beat,int leads_to_room_id) : Entity()
  {
    type_ = type;
    locked_=locked;
    dice_to_beat_=dice_to_beat;
    leads_to_room_id_ = leads_to_room_id;
  }

  std::string getId() const override
  {
    return type_;
  }

  void setLeadsToRoomPtr(Room *ptr) override
  {
    leads_to_room_ptr_=ptr;
  }

  std::map<Item*, int> getLoot() override
  {
    return loot_;
  }

  void addLoot(Item* item, int amount) override
  {
    loot_.insert({item,amount});
  }
  void eraseLoot(Item* item) override
  {
    loot_.erase(item);
  }

  int getNumber() const override {return leads_to_room_id_;}

  PositionLocked(const PositionLocked&) = delete;

  ~PositionLocked() = default;

  bool getIfLocked() override {return locked_;}

  int leadsTo() override
  {
    return leads_to_room_id_;
  }

  int getDiceToBeat() override
  {
    return dice_to_beat_;
  }

  void setLock(bool a) override
  {
    locked_ = a;
  }

  void printLoot() override;
};
#endif
