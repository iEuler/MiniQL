#ifndef MINIQL_FORWARDSTRUCTURE_HPP
#define MINIQL_FORWARDSTRUCTURE_HPP

#include <cmath>

#include "../../error.hpp"
#include "../../interestrate.hpp"
#include "../yieldtermstructure.hpp"
#include "../../typedef.hpp"
#include "../../time/date.hpp"

namespace MiniQL {

class ForwardRateStructure : public YieldTermStructure {
  public:
    ForwardRateStructure() = default;
    ForwardRateStructure(const Date& referenceDate,
                  const Calendar& calendar = Calendar(),
                  const DayCounter& dc = DayCounter())
      : YieldTermStructure(referenceDate, calendar, dc) {}    
    ~ForwardRateStructure() override = default;

  protected:    
    // forward-rate calculation    
    virtual Rate forwardRateImpl(Time t) const = 0;

    // zero-yield calculation
    virtual Rate zeroYieldImpl(Time t) const {
      if (t < epsilon)
        return forwardRateImpl(0.0);
      
      int N = 100;
      Time dt = t/N;
      Rate sums = 0.5*forwardRateImpl(0.0);
      for (int i=1; i<N; i++)
        sums += forwardRateImpl(dt*i);
      sums += forwardRateImpl(t);
      return Rate(sums * dt / t);

      

    }

    DiscountFactor discountImpl(Time t) const override {
      if (t < epsilon) 
        return 1.0;
      Rate r = zeroYieldImpl(t);
      return DiscountFactor(std::exp( - r*t));
    }
};

}  // namespace MiniQL

#endif