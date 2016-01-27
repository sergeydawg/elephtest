#include "random.h"

namespace pclengine
{

RandomUtils *RandomUtils::s_random = nullptr;

RandomUtils* RandomUtils::getInstance()
{
  if(s_random == nullptr)
    s_random = new (std::nothrow) RandomUtils();

  return s_random;
}

void RandomUtils::destroyInstance()
{
  delete s_random;
  s_random = nullptr;
}

Random& RandomUtils::get()
{
  return _rnd;
}

} // end namespace pclengine
