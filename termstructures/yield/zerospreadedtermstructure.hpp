#ifndef MINIQL_ZEROSPREADEDTERMSTRUCTURE_HPP
#define MINIQL_ZEROSPREADEDTERMSTRUCTURE_HPP

#include "zeroyieldstructure.hpp"
#include "../yieldtermstructure.hpp"
#include "../../error.hpp"
#include "../../handle.hpp"
#include "../../interestrate.hpp"
#include "../../quote.hpp"
#include "../../typedef.hpp"
#include "../../time/date.hpp"

namespace MiniQL {

class ZeroSpreadedTermStructure : public ZeroYieldStructure {
  public:
    ZeroSpreadedTermStructure(Handle<YieldTermStructure> originalCurve,
                              Handle<Quote> spread)                              
    : originalCurve_(std::move(originalCurve)), spread_(std::move(spread))    
    {
      registerWith(originalCurve_);
      registerWith(spread_);
    }

    const Date& referenceDate() const override {
      originalCurve_->referenceDate();
    }

    // void update() override {  }

    Rate zeroYieldImpl(Time t) const override {
      InterestRate zeroRate = originalCurve_->zeroRate(t, Compounding::Continuous);
      return Rate(zeroRate) + spread_->value();
    }

    // Rate forwardImpl(Time t) const override {
    //   InterestRate fwdRate = originalCurve_->forwardRate(t, Compounding::Continuous);
    //   return Rate(fwdRate) + spread_->value();
    // }

  protected:
    Handle<YieldTermStructure> originalCurve_;
    Handle<Quote> spread_;
    // Compounding comp_;
    // Frequency freq_;
    // DayCounter dc_;
    
};

}  // namespace MiniQL

#endif