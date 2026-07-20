#ifndef SPELL_HPP
#define SPELL_HPP

#include <vector>
#include <iostream>
#include <algorithm>

#include "Card.hpp"
#include "Creature.hpp"
#include "GameError.hpp"
#include "Board.hpp"
#include "Utils.hpp"

enum SpellType
{
  GENERAL, TARGET, GRAVEYARD
};

class Spell : public Card
{
  private:
    SpellType type_;
  public:
    Spell(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost, SpellType type);
    Spell(const Spell&) = delete;
    virtual ~Spell() override = default;


    bool isCreature() const override;
    std::vector<std::string> spellTarget(const std::vector<std::string>& parameters);
    virtual void executeSpell(const std::vector<std::string>& parameters) = 0;


    std::shared_ptr<Creature> checkAndReturnTarget(const std::vector<std::string> target) const;
    std::shared_ptr<Card> checkAndReturnGrave(const std::string& id) const;

    std::string getCardRowString(int row) const override;
    SpellType getType() const;
};

class BattleCry : public Spell
{
  public:
    BattleCry(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::GENERAL) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~BattleCry() override = default;
};

class Meteor : public Spell
{
  public:
    Meteor(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::GENERAL) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~Meteor() override = default;
};

class Fireball : public Spell
{
  public:
    Fireball(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::GENERAL) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~Fireball() override = default;
};

class HeroicMem : public Spell
{
  public:
    HeroicMem(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::GRAVEYARD) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~HeroicMem() override = default;
};

class Revive : public Spell
{
  public:
    Revive(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::GRAVEYARD) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~Revive() override = default;
};

class Clone : public Spell
{
  public:
    Clone(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::TARGET) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~Clone() override = default;
};

class DeathCurse : public Spell
{
  public:
    DeathCurse(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::TARGET) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~DeathCurse() override = default;
};

class Shock : public Spell
{
  public:
    Shock(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::TARGET) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~Shock() override = default;
};

class Mobilize : public Spell
{
  public:
    Mobilize(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::TARGET) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~Mobilize() override = default;
};

class RapidRush : public Spell
{
  public:
    RapidRush(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::TARGET) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~RapidRush() override = default;
};

class Shield : public Spell
{
  public:
    Shield(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::TARGET) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~Shield() override = default;
};

class Amputate : public Spell
{
  public:
    Amputate(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::TARGET) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~Amputate() override = default;
};

class Final : public Spell
{
  public:
    Final(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::TARGET) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~Final() override = default;
};

class Loyalty : public Spell
{
  public:
    Loyalty(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::TARGET) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~Loyalty() override = default;
};

class Zombify : public Spell
{
  public:
    Zombify(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::TARGET) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~Zombify() override = default;
};

class Bloodlust : public Spell
{
  public:
    Bloodlust(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost) :
      Spell(owner, id, name, manacost, SpellType::TARGET) {}

    void executeSpell(const std::vector<std::string>& parameters) override;

    ~Bloodlust() override = default;
};

#endif
