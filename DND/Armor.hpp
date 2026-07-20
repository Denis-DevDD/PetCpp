

#ifndef ARMOR_HPP
#define ARMOR_HPP

#include "Item.hpp"
#include "Character.hpp"
#include "Entity.hpp"

class Armor : public Item
{
  private:
  Armortype armtype;
  int armor_value_;
  std::string full_name_;

  public:
  Armortype nametoArmtype(std:: string& name);
  std::string nameToFullName(std::string name);
  Armor (std::string name);
  virtual void setArmValue(Entity* Char) override;

  ~Armor() override {}

  Armor(const Armor&)=delete;

  static std::string aNameToName(Armortype);

  virtual std::string getFullName() const override;
  virtual int getArmValue() const override;
  virtual Armortype getArmName() override;
};

#endif
