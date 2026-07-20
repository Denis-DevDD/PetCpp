
#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <string>

namespace Oop
{
  class Random final
  {
    public:
      static Random& getInstance();

      Random(Random const&) = delete;

      Random& operator=(Random const&) = delete;

      size_t getRandomNumber(size_t const upper_bound);

    private:
      std::mt19937 mersenne_twister_;

      static std::string const SEED;

      Random();

      size_t getSeed(std::string const environment_variable = nullptr);

      size_t getEnvironmentSeed(std::string const environment_variable);

      size_t getHardwareSeed();

      size_t parseSeed(std::string const seed);
  };
}

#endif
