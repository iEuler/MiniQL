#ifndef MINIQL_YIELDTERMSTRUCTURE_HPP
#define MINIQL_YIELDTERMSTRUCTURE_HPP

#include <cmath>

#include "../error.hpp"
#include "../interestrate.hpp"
#include "../termstructure.hpp"
#include "../typedef.hpp"
#include "../time/date.hpp"

namespace MiniQL {

class YieldTermStructure : public TermStructure {
  public:
    YieldTermStructure() = default;
    YieldTermStructure(const Date& referenceDate,
                  const Calendar& calendar = Calendar(),
                  const DayCounter& dc = DayCounter())
      : TermStructure(referenceDate, calendar, dc) {}    
    ~YieldTermStructure() override = default;

    DiscountFactor discount(Time t) const { return discountImpl(t); }
    DiscountFactor discount(const Date& d) const { return discount(timeFromReference(d)); }
    
    InterestRate zeroRate(Time t,
                          Compounding comp = Compounding::Simple,
                          Frequency freq = Frequency::Annual) 
    {
      Real compoundedRate = 1.0/discount(t);
      return InterestRate::impliedRate(compoundedRate, t, dayCounter_, comp, freq);
    }
    InterestRate zeroRate(const Date& d,
                          Compounding comp = Compounding::Simple,
                          Frequency freq = Frequency::Annual) 
    {
      return zeroRate(timeFromReference(d), comp, freq);      
    }

    InterestRate forwardRate(Time t1,
                             Time t2,
                             Compounding comp = Compounding::Simple,
                             Frequency freq = Frequency::Annual) 
    {      
      QL_REQUIRE(t1<=t2, t1 << " is later than " << t2);
      Real compoundedRate;
      Time dt = 0.0001;
      if (std::abs(t1-t2) < 1e-14) 
      {
        t1 = std::max(t1 - dt/2.0, 0.0);
        t2 = t1 + dt;
      }
      compoundedRate = discount(t1) / discount(t2);
      return InterestRate::impliedRate(compoundedRate, t2-t1, dayCounter_, comp, freq);
    }

    InterestRate forwardRate(const Date& d1,
                             const Date& d2,
                             Compounding comp = Compounding::Simple,
                             Frequency freq = Frequency::Annual) 
    {
      Time t1 = timeFromReference(d1);
      Time t2 = timeFromReference(d2);
      return forwardRate(t1, t2, comp, freq);
    }

    void update() override { TermStructure::update(); }

  protected:
    virtual DiscountFactor discountImpl(Time) const = 0;
};

}  // namespace MiniQL

#endif