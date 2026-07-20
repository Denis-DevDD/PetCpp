#ifndef CREATURE_HPP
#define CREATURE_HPP

#include <vector>
#include <algorithm>

#include "Card.hpp"

enum class Trait
{
  BRUTAL, CHALLENGER, FIRST_STRIKE, HASTE, LIFESTEAL, POISONED,
  REGENERATE, TEMPORARY, UNDYING, VENOMOUS
};

class Creature : public Card
{
  private:
    const int base_health_;
    int current_health_;
    const int base_attack_;
    int current_attack_;
    std::vector<Trait> base_traits_;
    std::vector<Trait> current_traits_;
    bool is_locked_;

  public:
    Creature(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost,
      int health, int attack, std::vector<Trait> traits);
    Creature(const Creature&) = default;
    ~Creature() override = default;

    bool isCreature() const override;
    bool hasTrait(Trait trait) const;
    void addTrait(Trait trait);
    void changeStats(int attack, int health);
    void resetStats();
    void removeTrait(Trait trait);
    void lockCard() { is_locked_ = true; }
    void unlockCard() { is_locked_ = false; }
    bool isLocked() { return is_locked_; }
    void resetCard();
    void regenerate() { current_health_ = base_health_; }
    void heal(int health_point) { current_health_ += health_point; }
    void takeDamage(int damage) { current_health_ -= damage; }
    void removeAlphabeticallyFirstTrait();

    std::vector<int> getStats() const;
    std::vector<Trait> getTraits() const;
    int getBaseHealth() { return base_health_; }
    int getCurrentHealth() { return current_health_; }
    int getCurrentAttack() const { return current_attack_; }
    std::string getCardRowString(int row) const override;
    std::string getTraitsToPrint() const;
};

#endif
