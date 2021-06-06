#ifndef MINIQL_QUOTE_HPP
#define MINIQL_QUOTE_HPP

#include "typedef.hpp"
#include "patterns/observable.hpp"

namespace MiniQL {

class Quote: public virtual Observable {
  public:
    ~Quote() override = default;
    virtual Real value() const = 0;    
};

}  // namespace MiniQL

#endif