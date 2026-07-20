#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include <vector>
#include <memory>

class Player;
class Card
{
  protected:
    std::weak_ptr<Player> owner_;
    std::string id_;
    std::string name_;
    int manacost_;

  public:
    Card(std::shared_ptr<Player> owner, const std::string& id, const std::string& name, int manacost);
    Card(const Card&) = default;
    virtual ~Card() = default;

    virtual bool isCreature() const = 0;
    void setMana(int new_mana);

    std::weak_ptr<Player> getOwner() const;
    std::string getId() const;
    std::string getName() const;
    int getManaCost() const;
    virtual std::string getCardRowString(int row) const = 0;
};

#endif
