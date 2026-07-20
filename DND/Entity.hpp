#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <string>
#include "Item.hpp"
#include "Weapon.hpp"
#include <map>

class Room;

class Entity
{
protected:
  int posX_;
  int posY_;
  bool position_lock_;
  bool if_player_;
  DamageType resist_;
  static std::map<std::string, Item*>* all_items_;
public:
  void setPos(int x,int y);

  void setPosLock(bool a);

  virtual std::string getId() const;

  virtual void setId(std::string);

  virtual void setLeadsToRoomPtr(Room*);

  static void setAllItems(std::map<std::string, Item*>* all_items);

  static std::map<std::string, Item*>* getAllItems();

  int getX();

  int getY();

  virtual int getNumber() const;

  virtual bool getIfLocked();

  Entity() = default;

  Entity(const Entity&) = delete;

  virtual ~Entity();

  virtual int leadsTo();

  virtual std::map<Item*, int> getLoot();

  virtual std::map<Item*, int> getInventory();

  virtual int getHP() const;

  DamageType getRes() const;

  virtual void addInventory(Item*, int);

  virtual void remInventory(Item*, int);

  virtual void setHp(int);

  virtual void eraseLoot(Item*);

  virtual Item* getEquippeda();

  virtual void setEquippeda(Item*);

  virtual Item* getEquippedw();

  virtual void setEquippedw(Item*);

  virtual int getMaxHP() const;

  virtual std::string getName() const;

  virtual int getVital() const;

  virtual int getStr() const;

  virtual std::string getClassName() const;

  virtual int getArmorVal() const;

  virtual bool getIfPositionLocked() const;

  virtual int getDiceToBeat();

  virtual void setLock(bool);

  virtual void printLoot();

  virtual void addLoot(Item*, int);

  virtual void remLoot(Item*, int);

  virtual void kill();

  virtual bool getifWasLooted();

  virtual void setifWasLooted(bool);

  virtual bool getIfplayer();

  virtual bool ifEnemy();

  virtual void fromXtoId();

  void setResist(DamageType res);

  virtual void setIfFleed(bool) {};
};
#endif
