#ifndef MINIQL_VANILLAOPTION_HPP
#define MINIQL_VANILLAOPTION_HPP

#include "../exercise.hpp"
#include "../handle.hpp"
#include "../instrument.hpp"
#include "../payoff.hpp"
#include "../pricingengine.hpp"
#include "../settings.hpp"
#include "../typedef.hpp"
#include "swap.hpp" // to use YieldTermStructure

namespace MiniQL {


class VanillaOption : public Instrument {
  public:
    // accessory classes
    class Arguments;
    class Results;
    class Engine;

    VanillaOption() = default;
    VanillaOption(const std::shared_ptr<Payoff>& payoff,
                  const std::shared_ptr<Exercise>& exercise)
      : payoff_(payoff), exercise_(exercise) { }
    VanillaOption(const std::shared_ptr<Payoff>& payoff,
                  const std::shared_ptr<Exercise>& exercise,
                  const Handle<YieldTermStructure>& interestRate,
                  const Handle<YieldTermStructure>& volatility,
                  Real spot)
      : payoff_(payoff), exercise_(exercise), 
        interestRate_(interestRate), volatility_(volatility), 
        spot_(spot) { }
    ~VanillaOption() = default;

    bool isExpired() const override;
    // void setupArguments(Arguments *) const;   // Note this is not an override of Instrument::setupArguments
    // void fetchResults(const Results *) const; // Note this is not an override of Instrument::fetchResults
    void setupArguments(PricingEngine::Arguments *) const override;
    void fetchResults(const PricingEngine::Results *) const override;


    // add option-specific results, delta, gamma, theta...
    Real delta() const;
    
  protected:    
    void setupExpired() const;
    // option data
    std::shared_ptr<Payoff> payoff_;
    std::shared_ptr<Exercise> exercise_;
    mutable Real delta_;
    
    // below variables are for simplied version of VanillaOptions
    Handle<YieldTermStructure> interestRate_, volatility_;
    Real spot_;
    
};

class VanillaOption::Arguments : public PricingEngine::Arguments {
  public:
    Arguments() = default;
    void validate() const override {}  // TODO: to implement
    std::shared_ptr<Payoff> payoff;
    std::shared_ptr<Exercise> exercise;
    Handle<YieldTermStructure> interestRate, volatility;
    Real spot; 
};

class VanillaOption::Results : public Instrument::Results {
  public:
    Results() = default;
    void reset() override {}  // TODO: to implement
    Real delta;
};

class VanillaOption::Engine 
    : public GenericEngine<VanillaOption::Arguments, 
                           VanillaOption::Results> {
  public:
    Engine() = default;
    void calculate() const override {}
};

class AnalyticEuropeanEngine : public VanillaOption::Engine {
  public:
    void calculate() const override;

  private:
    // std::shared_ptr<>
};

inline bool VanillaOption::isExpired() const {
  Date today = Settings::instance().evaluationDate();
  return exercise_->lastDate() < today;
}

inline void VanillaOption::setupExpired() const {
  Instrument::setupExpired();
}

inline void VanillaOption::setupArguments(PricingEngine::Arguments* args) const {
  VanillaOption::Arguments* arguments = dynamic_cast<VanillaOption::Arguments*>(args);
  arguments->exercise = exercise_;
  arguments->payoff = payoff_;
  arguments->interestRate = interestRate_;
  arguments->volatility = volatility_;
  arguments->spot = spot_;

}

inline void VanillaOption::fetchResults(const PricingEngine::Results* r) const {
  Instrument::fetchResults(r);
  const VanillaOption::Results* results = dynamic_cast<const VanillaOption::Results*>(r);
  delta_ = results->delta;
}

inline Real VanillaOption::delta() const {
  calculate();
  return delta_;  
}

}  // namespace MiniQL

#endif 
