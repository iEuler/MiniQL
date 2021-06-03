#ifndef MINIQL_YIELDTERMSTRUCTURE_HPP
#define MINIQL_YIELDTERMSTRUCTURE_HPP

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
                          Compounding comp,
                          Frequency freq = Frequency::Annual) 
    {
      Real compoundedRate = 1.0/discount(t);
      return InterestRate::impliedRate(compoundedRate, t, dayCounter_, comp, freq);
    }
    InterestRate zeroRate(const Date& d,
                          Compounding comp,
                          Frequency freq = Frequency::Annual) 
    {
      return zeroRate(timeFromReference(d), comp, freq);      
    }

    void update() override;

  protected:
    virtual DiscountFactor discountImpl(Time) const = 0;
};

}  // namespace MiniQL

#endif