#include "Weapon.hpp"
#include "Random.hpp"
#include "Character.hpp"

size_t Weapon::rand(int max)
{
  using namespace Oop;
  rolls_++;
  last_max = max;
  return Random::getInstance().getRandomNumber(max);
}

Weapon::Weapon(std::string name, AttackType atcktype, DamageType dmgtp, DamagePattern dmgptrn) : Item(name)
{
  w_name_ = nametoWname(name);
  full_name_ = nametoFullName(name);
  attack_type_ = atcktype;
  damage_type_ = dmgtp;
  damage_pattern_ = dmgptrn;
  damage_amount_ = 0;
  item_type_ = ItemType::WEAPON;
  sub_type_ = SubWeaponType::MELEE;
  sub_type_ = nameToSubType(w_name_);
}

WeaponName Weapon::nametoWname(std::string& name)
{
  std::map<std::string, WeaponName> wtransfer={ {"DAGG", WeaponName::DAGG},
  {"MACE", WeaponName::MACE}, {"WARH", WeaponName::WARH}, {"HAXE", WeaponName::HAXE},
  {"GAXE", WeaponName::GAXE}, {"RAPI", WeaponName::RAPI}, {"SSWD", WeaponName::SSWD},
  {"LSWD", WeaponName::LSWD}, {"GSWD", WeaponName::GSWD}, {"SBOW", WeaponName::SBOW},
  {"LBOW", WeaponName::LBOW}, {"LCRS", WeaponName::LCRS}, {"HCRS", WeaponName::HCRS},
  {"QFIR", WeaponName::QFIR}, {"QCLD", WeaponName::QCLD}, {"QACD", WeaponName::QACD},
  {"QFRC", WeaponName::QFRC}
  };
  auto pair = wtransfer.find(name);
  return pair->second;
}
std::string Weapon::nametoFullName(std::string& name)
{
  std::map<std::string, std::string> wtransfer={ {"DAGG", "Dagger"},
  {"MACE", "Mace"}, {"WARH", "Warhammer"}, {"HAXE", "Handaxe"}, {"GAXE", "Greataxe"},
  {"RAPI", "Rapier"}, {"SSWD", "Shortsword"}, {"LSWD", "Longsword"}, {"GSWD", "Greatsword"},
  {"SBOW", "Shortbow"}, {"LBOW", "Longbow"}, {"LCRS", "Light Crossbow"}, {"HCRS", "Heavy Crossbow"},
  {"QFIR", "Quarterstaff of Fire"}, {"QCLD", "Quarterstaff of Cold"},
  {"QACD", "Quarterstaff of Acid"}, {"QFRC", "Quarterstaff of Force"},
  };
  auto pair = wtransfer.find(name);
  return pair->second;
}
int Weapon::damagecalc(Character* chr)
{
  rolls_ = 0;
  int dmg = 0;
  result_ = 0;
  switch (w_name_)
  {
    case WeaponName::DAGG:
    dmg = (result_ = rand(4)) + chr->getVital();
    break;
    case WeaponName::MACE:
    dmg = (result_ = rand(6)) + chr->getStr();
    break;
    case WeaponName::WARH:
    dmg = (result_ = rand(10)) + chr->getStr();
    break;
    case WeaponName::HAXE:
    dmg = (result_ = rand(6)) + chr->getStr();
    break;
    case WeaponName::GAXE:
    dmg = (result_ = rand(12)) + chr->getStr();
    break;
    case WeaponName::RAPI:
    dmg = (result_ = rand(8)) + chr->getVital();
    break;
    case WeaponName::SSWD:
    dmg = (result_ = rand(6)) + chr->getVital();
    break;
    case WeaponName::LSWD:
    dmg = (result_ = rand(10)) + chr->getVital();
    break;
    case WeaponName::GSWD:
    dmg = (result_ = rand(6)) + rand(6) + chr->getStr();
    break;
    case WeaponName::SBOW:
    dmg = (result_ = rand(6)) + chr->getVital();
    break;
    case WeaponName::LBOW:
    dmg = (result_ = rand(8)) + chr->getVital();
    break;
    case WeaponName::LCRS:
    dmg = (result_ = rand(8)) + chr->getVital();
    break;
    case WeaponName::HCRS:
    dmg = (result_ = rand(10)) + chr->getVital();
    break;
    default:
    break;
  }
  if (chr->getId()=="W" || chr->getId()=="L" )
  {
    switch (w_name_)
    {
      case WeaponName::QFIR :
      dmg = (result_ = rand(6) + rand(6) + rand(6));
      break;
      case WeaponName::QCLD :
      dmg = (result_ = rand(10) + rand(10));
      break;
      case WeaponName::QACD :
      dmg = (result_ = rand(10));
      break;
      case WeaponName::QFRC :
      dmg = (result_ = rand(10));
      break;
      default:
      break;
    }
  }
  else
  {
    switch (w_name_)
    {
      case WeaponName::QFIR :
      dmg = (result_ = rand(1)) + chr->getStr();
      break;
      case WeaponName::QCLD :
      dmg = (result_ = rand(1)) + chr->getStr();
      break ;
      case WeaponName::QACD :
      dmg = (result_ = rand(1)) + chr->getStr();
      break;
      case WeaponName::QFRC :
      dmg = (result_ = rand(1)) + chr->getStr();
      break;
      default:
      break;
    }

  }

  std::cout<<"[Dice Roll] "<<rolls_<<" d"<<last_max
  <<" resulting in a total value of "<<result_<<"."<<std::endl<<std::endl;
  return dmg;
}

SubWeaponType Weapon::nameToSubType(WeaponName name)
{
  switch (name)
  {
    case WeaponName::SBOW:
    case WeaponName::LBOW:
    return SubWeaponType::BOW;
    break;
    case WeaponName::LCRS:
    case WeaponName::HCRS:
    return SubWeaponType::CROSSBOW;
    break;
  default:
  return SubWeaponType::MELEE;
  break;
  }
}

std::string Weapon::wNameToName(WeaponName wname)
{
  std::map<WeaponName, std::string> reverse_wtransfer = {
    {WeaponName::DAGG, "DAGG"}, {WeaponName::MACE, "MACE"}, {WeaponName::WARH, "WARH"},
    {WeaponName::HAXE, "HAXE"}, {WeaponName::GAXE, "GAXE"}, {WeaponName::RAPI, "RAPI"},
    {WeaponName::SSWD, "SSWD"}, {WeaponName::LSWD, "LSWD"}, {WeaponName::GSWD, "GSWD"},
    {WeaponName::SBOW, "SBOW"}, {WeaponName::LBOW, "LBOW"}, {WeaponName::LCRS, "LCRS"},
    {WeaponName::HCRS, "HCRS"}, {WeaponName::QFIR, "QFIR"}, {WeaponName::QCLD, "QCLD"},
    {WeaponName::QACD, "QACD"}, {WeaponName::QFRC, "QFRC"}
};
  auto pair = reverse_wtransfer.find(wname);
  if (pair == reverse_wtransfer.end())
  {
    return "NONE";
  }
  return pair->second;
}
