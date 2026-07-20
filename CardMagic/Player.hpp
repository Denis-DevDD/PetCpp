#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "Card.hpp"
#include "ManaPool.hpp"

enum class Role
{
  ATTACKER,
  DEFENDER
};

class Player
{
  private:
    int health_;
    Role role_;
    std::vector<std::shared_ptr<Card>> deck_;
    std::vector<std::shared_ptr<Card>> graveyard_;
    std::vector<std::shared_ptr<Card>> hand_;
    ManaPool mana_pool_;

    bool is_winner_;
    bool redraw_status_;

  public:
    Player(int health, int mana_pool_size);
    Player(const Player&) = delete;
    ~Player() = default;

    bool drawCards(int num_to_draw);
    void setRole(Role role);
    void makeWinner() { is_winner_ = true; }
    bool isWinner() { return is_winner_; }

    void addCardToDeck(std::shared_ptr<Card> card) { deck_.push_back(card); }
    void addCardToHand(std::shared_ptr<Card> card) { hand_.push_back(card); }
    void addToGrave(std::shared_ptr<Card> addCard);
    void removeTopDeckCard() { deck_.erase(deck_.begin()); }
    void removeTopHandCard() { hand_.erase(hand_.begin()); }
    std::shared_ptr<Card> removeCardFromHand(const std::string& card_id);
    void removeFromGraveyard(const std::shared_ptr<Card>& card);
    void clearHand() { hand_.clear(); }
    bool isCardInHand(const std::string& card_id);

    void disableRedrow();
    void printStatus() const;
    void takeDamage(int damage) { health_ -= damage; }

    const std::vector<std::shared_ptr<Card>>& getGraveyard() const { return graveyard_; }
    const std::vector<std::shared_ptr<Card>>& getDeck() const { return deck_; }
    const std::vector<std::shared_ptr<Card>>& getHand() const { return hand_; }
    int getHealth() const { return health_; }
    bool getRedrawStatus() const { return redraw_status_; }
    ManaPool& getManaPool() { return mana_pool_; };
    Role getRole() const { return role_; }
    int getDeckSize() const {return deck_.size(); }
    int getGraveyardSize() { return graveyard_.size(); }
};

#endif
