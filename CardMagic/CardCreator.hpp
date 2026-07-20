#ifndef CARDCREATOR_HPP
#define CARDCREATOR_HPP

#include <memory>

#include "Player.hpp"
#include "Creature.hpp"
#include "Spell.hpp"
#include "CardDatabase.hpp"

class CardCreator
{
  public:
    static std::shared_ptr<Card> createCardById(const std::string& id, std::shared_ptr<Player> owner)
    {
      CardData data = *CardDatabase::getInfo(id);
      if (data.type_ == CardType::CREATURE)
      {
        return std::make_shared<Creature>(owner, data.id_, data.name_, data.mana_cost_,
          data.base_health_, data.base_attack_, data.traits_);
      }

      if (id == "BTLCY")
      {
        return std::make_shared<BattleCry>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "METOR")
      {
        return std::make_shared<Meteor>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "FIRBL")
      {
        return std::make_shared<Fireball>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "CLONE")
      {
        return std::make_shared<Clone>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "CURSE")
      {
        return std::make_shared<DeathCurse>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "SHOCK")
      {
        return std::make_shared<Shock>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "MOBLZ")
      {
        return std::make_shared<Mobilize>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "RRUSH")
      {
        return std::make_shared<RapidRush>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "SHILD")
      {
        return std::make_shared<Shield>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "AMPUT")
      {
        return std::make_shared<Amputate>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "FINAL")
      {
        return std::make_shared<Final>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "LYLTY")
      {
        return std::make_shared<Loyalty>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "ZMBFY")
      {
        return std::make_shared<Zombify>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "BLOOD")
      {
        return std::make_shared<Bloodlust>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "MEMRY")
      {
        return std::make_shared<HeroicMem>(owner, data.id_, data.name_, data.mana_cost_);
      }
      if (id == "REVIV")
      {
        return std::make_shared<Revive>(owner, data.id_, data.name_, data.mana_cost_);
      }

      else
      {
        return nullptr;
      }
    }
};

#endif
