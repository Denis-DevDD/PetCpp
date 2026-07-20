#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <map>
#include "Item.hpp"
#include <algorithm>
#include "Entity.hpp"

class Weapon;

class Character : public Entity
{
protected:
  std::string type_id_;
  int max_hp_;
  int current_hp_;
  int armor_value_;
  int strength_;
  int vitality_;
  bool alive_;
  bool if_looted_;
  std::map<Item*, int> inventory_;
  Item* equipped_weapon_;
  Item* equipped_armor_;
  std::map<Item*,int> loot_;

public:
  Character(int max_hp,int armor,int str,int vit);

  Character(const Character&) = delete;

  void kill() override;
  virtual void setId(std::string id) override;
  virtual void setHp(int hp) override;

  void addInventory(Item* item, int amount) override;

  void remInventory(Item* item, int amount) override;

  static bool compareIt(const std::pair<Item*, int>& a, const std::pair<Item*, int>& b);

  void printLoot() override;


  void addLoot(Item* item, int amount) override;

  void remLoot(Item* item, int amount) override;


  void setEquippeda(Item* item) override;

  void setEquippedw(Item* item) override;


  void setifWasLooted(bool if_looted) override;

  virtual void fromXtoId() override {};


  virtual std::string getId() const override;

  virtual int getHP() const override;

  int getVital() const override;

  int getStr() const override;

  std::map<Item*,int> getLoot() override;

  Item* getEquippedw() override;

  virtual Item* getEquippeda() override;

  std::map<Item*, int> getInventory() override;

  int getArmorVal() const override;

  int getMaxHP() const override;

  virtual std::string getName() const override;

  virtual std::string getClassName() const override;

  virtual bool getIfPositionLocked() const override;

  bool getIfDead() const;

  bool ifEnemy() override;

  bool getifWasLooted() override;

  virtual ~Character() = default;
};


#endif
