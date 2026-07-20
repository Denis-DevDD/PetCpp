#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <vector>
#include <iostream>
class Character;
class Player;
class Entity;
enum class WeaponName
{
  DAGG, MACE, WARH, HAXE, GAXE, RAPI, SSWD, LSWD, GSWD, SBOW, LBOW,
  LCRS, HCRS, QFIR, QCLD, QACD, QFRC
};

enum class Armortype
{
  LARM = 1, BPLT = 4, CHML = 6, PARM = 8
};
enum class SubWeaponType
{
  BOW, CROSSBOW, MELEE
};
enum class DamageType
{
  PHYSICAL, COLD, FIRE, ACID, FORCE, NONE
};
enum class ItemType
{
  WEAPON, CONSUMABLE, ARMOR
};
enum class AttackType
{
  MELEE, RANGED
};
enum class DamagePattern
{
  HIT, THRUST, SLASH, LINE, SHOT, BURST
};
class Item
{
protected:
  std::string name_;
  ItemType item_type_;
public:

  Item(std::string& name) {name_ = name;}

  Item(const Item&) = delete;

  std::string getName()
  {
    return name_;
  }
  virtual ~Item() {}

  virtual SubWeaponType getSubType() {return SubWeaponType::MELEE;}
  virtual DamageType getDType(){return DamageType::NONE;}
  virtual AttackType getaType(){return AttackType::MELEE;}
  virtual int damagecalc(Character*){return 0;}
  virtual DamagePattern getPattern() {return DamagePattern::HIT;}
  virtual int getArmValue() const {return 0;}
  ItemType getItemType() const {return item_type_;}
  virtual std::string getFullName() const {return "None";}
  virtual void setArmValue(Entity*) {}
  virtual void usePotion(Player*) {};

  virtual WeaponName getWName() {return WeaponName::DAGG;};

  virtual Armortype getArmName() {return Armortype::BPLT;};

  virtual DamageType getDmgREsistanmces() {return DamageType::NONE;}
};



#endif
