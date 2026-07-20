#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include <string>
#include "Character.hpp"
#include "Field.hpp"

class Player : public Character
{
protected:
  std::string name_;
  std::string name_of_class_;
  bool fleed_;
public:
  Player(std::string name, int max_hp, int armor, int str, int vit, std::string id);

  bool checkAmmunition(std::string name);

  std::string getName() const override
  {
    return name_;
  }

  std::string getClassName() const override
  {
    return name_of_class_;
  }

  std::string getId() const override
  {
    return type_id_;
  }

  std::map<Item*, int>* getInv()
  {
    std::map<Item*, int>* inventory = &inventory_;
    return inventory;
  }

  bool getIfFleed() {return fleed_;}

  void setIfFleed(bool fleed) override {fleed_ = fleed;}

  Player(const Player&) = delete;

  ~Player() override = default;

  Field* findMySelf(Room* room);

  void fromXtoId() override;
};




#endif
