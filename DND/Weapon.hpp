#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "Item.hpp"
#include <map>


class Weapon : public Item
{
protected:
  std::string full_name_;
  WeaponName w_name_;
  AttackType attack_type_;
  DamageType damage_type_;
  DamagePattern damage_pattern_;
  SubWeaponType sub_type_;
  int damage_amount_;
  int rolls_;
  int last_max;
  int result_;

public:
  std::string getFullName() const override {return full_name_;}

  WeaponName nametoWname(std::string& name);

  std::string nametoFullName(std::string& name);

  SubWeaponType nameToSubType(WeaponName);

  Weapon(std::string name, AttackType atcktype, DamageType dmgtp, DamagePattern dmgptrn);

  SubWeaponType getSubType() override {return SubWeaponType::MELEE;}

  int damagecalc(Character* chr) override;

  AttackType getaType() override
  {
    return attack_type_;
  }

  WeaponName getWName() override
  {
    return w_name_;
  }

  DamagePattern getPattern() override
  {
    return damage_pattern_;
  }

  DamageType getDType() override
  {
    return damage_type_;
  }

  Weapon(const Weapon&) = delete;

  virtual ~Weapon() {};

  size_t rand(int max);

  static std::string wNameToName (WeaponName);
};


#endif
