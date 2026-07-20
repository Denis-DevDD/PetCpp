#include "Card.hpp"

Card::Card(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
  owner_(owner), id_(id), name_(name), manacost_(manacost) {}

void Card::setMana(int new_mana)
{
  manacost_ = new_mana;
}

std::weak_ptr<Player> Card::getOwner() const
{
  return owner_;
}

std::string Card::getId() const
{
  return id_;
}

std::string Card::getName() const
{
  return name_;
}

int Card::getManaCost() const
{
  return manacost_;
}
