#ifndef MINIQL_ZEROYIELDSTRUCTURE_HPP
#define MINIQL_ZEROYIELDSTRUCTURE_HPP

#include <cmath>

#include "../../error.hpp"
#include "../../interestrate.hpp"
#include "../yieldtermstructure.hpp"
#include "../../typedef.hpp"
#include "../../time/date.hpp"

namespace MiniQL {

class ZeroYieldStructure : public YieldTermStructure {
  public:
    ZeroYieldStructure() = default;
    ZeroYieldStructure(const Date& referenceDate,
                       const Calendar& calendar = Calendar(),
                       const DayCounter& dc = DayCounter())
    : YieldTermStructure(referenceDate, calendar, dc) {}    
    ~ZeroYieldStructure() override = default;

  protected:    
    // zero-yield calculation
    virtual Rate zeroYieldImpl(Time t) const = 0;

    DiscountFactor discountImpl(Time t) const override {
      if (t < QL_EPSILON) 
        return 1.0;
      Rate r = zeroYieldImpl(t);
      return DiscountFactor(std::exp( - r*t));
    }
};

}  // namespace MiniQL

#endif