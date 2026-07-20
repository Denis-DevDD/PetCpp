#include "Entity.hpp"

void Entity::setPos(int x,int y)
{
  posX_=x;
  posY_=y;
}

void Entity::setPosLock(bool a)
{
  position_lock_=a;
}

std::string Entity::getId() const
{
  return "Q";
}

void Entity::setId(std::string)
{
  return;
}

void Entity::setLeadsToRoomPtr(Room*)
{
  return;
}

std::map<std::string, Item*>* Entity::getAllItems()
{
  return all_items_;
}

int Entity::getX()
{
  return posX_;
}

int Entity::getY()
{
  return posY_;
}

int Entity::getNumber() const
{
  return -1;
}

bool Entity::getIfLocked()
{
  return false;
}

Entity::~Entity()
{
  if_player_ = false;
}

int Entity::leadsTo()
{
  return -1;
}

std::map<Item*,int> Entity::getLoot()
{
  std::map<Item*,int> map;
  map.insert({nullptr, 0});
  return map;
}

std::map<Item*, int> Entity::getInventory()
{
  std::map<Item*,int> map;
  map.insert({nullptr, 0});
  return map;
}

std::map<std::string, Item*>* Entity::all_items_ = nullptr;

void Entity::setAllItems(std::map<std::string, Item*>* all_items)
{
  all_items_ = all_items;
}

int Entity::getHP() const
{
  return 0;
}

DamageType Entity::getRes() const
{
  return resist_;
}

void Entity::addInventory(Item*, int) {}

void Entity::remInventory(Item*, int) {}

void Entity::setHp(int) {}

void Entity::eraseLoot(Item*){}

Item* Entity::getEquippeda()
{
  return nullptr;
}

void Entity::setEquippeda(Item*) {}

Item* Entity::getEquippedw()
{
  return nullptr;
}

void Entity::setEquippedw(Item*) {}

int Entity::getMaxHP() const
{
  return 0;
}

std::string Entity::getName() const
{
  return "no_name";
}

int Entity::getVital() const
{
  return -1;
}

int Entity::getStr() const
{
  return -1;
}

std::string Entity::getClassName() const
{
  return "no_name";
}

int Entity::getArmorVal() const
{
  return -1;
}

bool Entity::getIfPositionLocked() const
{
  return true;
}

int Entity::getDiceToBeat()
{
  return -1;
}

void Entity::setLock(bool) {}

void Entity::printLoot() {};

void Entity::addLoot(Item*, int) {};

void Entity::remLoot(Item*, int) {};

void Entity::kill() {}

bool Entity::getifWasLooted()
{
  return false;
}

void Entity::setifWasLooted(bool) {}

bool Entity::getIfplayer()
{
  return if_player_;
}
bool Entity::ifEnemy()
{
  return false;
}
void Entity::fromXtoId() {};

void Entity::setResist(DamageType res)
{
  resist_=res;
}
