#ifndef MINIQL_PAYOFF_HPP
#define MINIQL_PAYOFF_HPP

#include <string>
#include "typedef.hpp"

namespace MiniQL {

class Payoff {
  public:
    Payoff() = default;
    // Payoff(const Date& date, const Real& amount) : date_(date), amount_(amount) {}
    virtual ~Payoff() = default;

    // return the value of the payoff given the value of the underlying
    // virtual Real operator()(Real price) const = 0;
    virtual std::string description() const = 0;
    
};

class TypePayoff : public Payoff {
  public:
    ~TypePayoff() = default;

    OptionType optionType() const { return type_; }
    std::string description() const override { return "Type Payoff"; }
    
  protected:
    explicit TypePayoff(OptionType type) : type_(type) { }
    OptionType type_;
};

class StrikedTypePayoff : public TypePayoff {
  public:    
    explicit StrikedTypePayoff(OptionType type, Real strike) 
        : TypePayoff(type), strike_(strike) {}    
    ~StrikedTypePayoff() = default;
    
    Real strike() const { return strike_; }
    std::string description() const override { 
      return "Striked Type Payoff with strike = " + std::to_string(strike_); 
    }
  protected:
    Real strike_;
};

}  // namespace MiniQL

#endif 
