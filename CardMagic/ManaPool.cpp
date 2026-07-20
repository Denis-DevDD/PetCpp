#include "ManaPool.hpp"

ManaPool::ManaPool(int size) : current_mana_(size), mana_pool_size_(size) {}

void ManaPool::increaseManaPool(int amount)
{
  mana_pool_size_ += amount;
}

void ManaPool::spend(int amount)
{
  current_mana_ -= amount;
}

void ManaPool::refill()
{
  current_mana_ = mana_pool_size_;
}

int ManaPool::getCurrentMana() const
{
  return current_mana_;
}

int ManaPool::getManaPoolSize() const
{
  return mana_pool_size_;
}
