#include "observable.hpp"

namespace MiniQL {

void Observable::notifyObservers()
{
  for (auto &ph : observers_)
  {
    ph->update();
  }
}

}  // namespace MiniQL