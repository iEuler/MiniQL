#ifndef MINIQL_SURVIVALPROBABILITYSTRUCTURE_HPP
#define MINIQL_SURVIVALPROBABILITYSTRUCTURE_HPP

#include "../time/date.hpp"
#include "../defaulttermstructure.hpp"
#include "../typedef.hpp"

namespace MiniQL {

class SurvivalProbabilityStructure : public DefaultProbabilityTermStructure {
  public:
    SurvivalProbabilityStructure() = default;
    SurvivalProbabilityStructure(const Date& referenceDate,
                                 const Calendar& calendar = Calendar(),
                                 const DayCounter& dc = DayCounter())
    : DefaultProbabilityTermStructure(referenceDate, calendar, dc) {}
    ~SurvivalProbabilityStructure() = default;

  protected:
    Real defaultDensityImpl(Time t) const override 
    {
      Time dt = 0.0001;
      Time t1 = std::max(t-dt, 0.0);
      Time t2 = t+dt;
      Probability p1 = survivalProbabilityImpl(t1);
      Probability p2 = survivalProbabilityImpl(t2);
      return (p1-p2) / (t2-t1);
    }
};

}  // namespace MiniQL

#endif