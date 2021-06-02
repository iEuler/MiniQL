#ifndef MINIQL_INSTRUMENT_HPP
#define MINIQL_INSTRUMENT_HPP

#include <any>
#include <map>
#include <string>

// #include "observable.hpp"
#include "patterns/lazyobject.hpp"
#include "pricingengine.hpp"
#include "typedef.hpp"

namespace MiniQL {

class Instrument : public LazyObject {
  public:
    class Results;
    Instrument();
    ~Instrument() {};

    Real NPV() const;
    const Date& valuationDate() const;
    virtual bool isExpired() const = 0;
    
    // // return any result returned by the pricing engine
    // template<typename T>
    // T result(const std::string& tag) const;
    // // return all results returned by the pricing engine
    // std::map<std::string, std::any>& addtionalResults() const;


    void setPricingEngine(const std::shared_ptr<PricingEngine>& engine);
    virtual void setupArguments(PricingEngine::Arguments *) const;
    virtual void fetchResults(const PricingEngine::Results *) const;

  protected:    
    void calculate() const override;
    void performCalculations() const override;
    virtual void setupExpired() const;
    mutable Real NPV_;
    mutable Date valuationDate_;
    std::shared_ptr<PricingEngine> engine_;
};

class Instrument::Results : public PricingEngine::Results {
  public:
    void reset() override {
      value = Real();
      valuationDate = Date();
    }
    
    Real value;
    Date valuationDate;
    
};

inline Instrument::Instrument() : NPV_(Real()), valuationDate_(Date()) {}

inline Real Instrument::NPV() const {
  calculate();
  return NPV_;
}

inline const Date& Instrument::valuationDate() const { 
  calculate();
  return valuationDate_; 
}
    

inline void Instrument::setPricingEngine(const std::shared_ptr<PricingEngine>& e) {
  if (engine_ != nullptr)
    unregisterWith(engine_);
  engine_ = e;
  if (engine_ != nullptr)
    registerWith(engine_);
  update();    
}

inline void Instrument::setupArguments(PricingEngine::Arguments * arg) const {
  // in derived class, write to DerivedEngine::Arguments
}


inline void Instrument::fetchResults(const PricingEngine::Results *r) const {
  const Instrument::Results *results = dynamic_cast<const Instrument::Results*>(r);
  NPV_ = results->value;
  valuationDate_ = results->valuationDate;
}

inline void Instrument::calculate() const 
{
  if (isExpired()) {
    setupExpired();
    calculated_ = true;
  } else {
    LazyObject::calculate();
  }  
  
}

inline void Instrument::setupExpired() const 
{
  NPV_ = Real();
  valuationDate_ = Date();
}

inline void Instrument::performCalculations() const
{
  engine_->reset();
  setupArguments(engine_->getArguments());
  engine_->getArguments()->validate();
  engine_->calculate();
  fetchResults(engine_->getResults());
}


}  // namespace MiniQL

#endif 
