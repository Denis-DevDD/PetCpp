#include "Player.hpp"

Player::Player(int health, int mana_pool_size) : health_(health),
  mana_pool_(mana_pool_size), is_winner_(false), redraw_status_(true) {}

bool Player::drawCards(int num_to_draw)
{
  if (static_cast<size_t>(num_to_draw) > deck_.size())
  {
    return false;
  }

  for (int i = 0; i < num_to_draw; i++)
  {
    hand_.push_back(deck_.front());
    deck_.erase(deck_.begin());
  }
  return true;
}

std::shared_ptr<Card> Player::removeCardFromHand(const std::string& card_id)
{
  for (auto card_it = hand_.begin(); card_it != hand_.end(); ++card_it)
  {
    if ((*card_it)->getId() == card_id)
    {
      std::shared_ptr<Card> card = *card_it;
      hand_.erase(card_it);
      return card;
    }
  }
  return nullptr;
}

bool Player::isCardInHand(const std::string& card_id)
{
  for (const auto& card : hand_)
  {
    if (card_id == card->getId())
    {
      return true;
    }
  }
  return false;
}

void Player::setRole(Role role)
{
  role_ = role;
}

void Player::addToGrave(std::shared_ptr<Card> addCard)
{
  graveyard_.push_back(addCard);
}

void Player::disableRedrow()
{
  if (redraw_status_)
  {
    redraw_status_ = false;
  }
}

void Player::printStatus() const
{
  std::cout << "Role: " << ((role_ == Role::DEFENDER) ? "Defender\n" : "Attacker\n");
  std::cout << "Health: " << health_ << std::endl;
  std::cout << "Mana: " << mana_pool_.getCurrentMana() << " / " << mana_pool_.getManaPoolSize() << std::endl;
  std::cout << "Remaining Deck: " << deck_.size() << " card(s)" << std::endl;
  std::cout << "Graveyard Size: " << graveyard_.size() << " card(s)" << std::endl;
}

void Player::removeFromGraveyard(const std::shared_ptr<Card>& card)
{
  auto it = std::find_if(graveyard_.begin(), graveyard_.end(), [&card](const std::shared_ptr<Card>& c)
  {
    return c == card;
  });
  if (it != graveyard_.end())
  {
    graveyard_.erase(it);
  }
}
