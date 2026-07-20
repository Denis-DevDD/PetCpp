#ifndef MANAPOOL_HPP
#define MANAPOOL_HPP

#include <iostream>
#include <string>
#include <vector>

class ManaPool
{
  private:
    int current_mana_;
    int mana_pool_size_;

  public:
    ManaPool(int size = 0);

    void increaseManaPool(int amount);
    void spend(int amount);
    void refill();

    int getCurrentMana() const;
    int getManaPoolSize() const;
};

#endif
