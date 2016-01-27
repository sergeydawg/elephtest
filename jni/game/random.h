//
//  random.h
//  elephtest
//
//  Created by Sergey Fatyunin on 22.01.16.
//
//

#ifndef __elephtest__random__
#define __elephtest__random__

#include "types.h"

#include <random>
#include <sstream>
#include <string>
#include <ctime>

namespace pclengine
{

class Random
{
public:
  void setSeed(u32 seed)
  {
    _generator.seed(seed);
  }

  u32 rand()
  {
    return _generator();
  }

  u32 urand(u32 min, u32 max)
  {
    std::uniform_int_distribution<u32> distribution(min,max);
    return distribution(_generator);
  }

  i32 irand(i32 min, i32 max)
  {
    std::uniform_int_distribution<i32> distribution(min,max);
    return distribution(_generator);
  }

  float frand(float min, float max)
  {
    std::uniform_real_distribution<float> distribution(min,max);
    return distribution(_generator);
  }

  Random(u32 seed=time(NULL))
    : _generator(seed)
  {
  }

private:
  std::mt19937 _generator;
};

class RandomUtils
{
public:
  static RandomUtils* getInstance();
  static void destroyInstance();

  Random& get();

private:
  RandomUtils(){}
  ~RandomUtils(){}

private:
  Random _rnd;
  static RandomUtils *s_random;
};

inline Random& RANDOM()
{
  return RandomUtils::getInstance()->get();
}

} // end namespace pclenginee

#endif
