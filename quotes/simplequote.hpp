#ifndef MINIQL_SIMPLEQUOTE_HPP
#define MINIQL_SIMPLEQUOTE_HPP

#include <memory>

#include "../quote.hpp"
#include "../typedef.hpp"

namespace MiniQL {

class SimpleQuote : public Quote {
  public:
    SimpleQuote(Real value = Real()) : value_(value) { }

    Real value() const override { return value_; }
    void setValue(Real value) { value_ = value; }
    void reset() { setValue(Real()); }

  private:
    Real value_;

};

}  // namespace MiniQL

#endif 
