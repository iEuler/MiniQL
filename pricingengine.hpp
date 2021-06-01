#ifndef MINIQL_PRICINGENGINE_HPP
#define MINIQL_PRICINGENGINE_HPP

#include "patterns/observable.hpp"

namespace MiniQL {

class PricingEngine : public virtual Observable { // an interface class
  public:
    class Arguments;
    class Results;    
    
    ~PricingEngine() override = default;

    virtual Arguments* getArguments() const = 0;
    virtual const Results* getResults() const = 0;
    virtual void reset() const = 0;
    virtual void calculate() const = 0; // in derived class, write to DerivedEngine::Results
};

class PricingEngine::Arguments {
  public:
    virtual ~Arguments() = default;
    virtual void validate() const = 0;
};

class PricingEngine::Results {
  public:
    virtual ~Results() = default;
    virtual void reset() = 0;
};

template<class ArgumentsType, class ResultsType>
class GenericEngine : public PricingEngine, public Observer {
  public:
    PricingEngine::Arguments* getArguments() const override { return &arguments_; }
    const PricingEngine::Results* getResults() const override { return &results_; }
    void reset() const override { results_.reset(); }
    void update() override { notifyObservers(); }

  protected:
    mutable ArgumentsType arguments_;
    mutable ResultsType results_;

};

}  // namespace MiniQL

#endif 
