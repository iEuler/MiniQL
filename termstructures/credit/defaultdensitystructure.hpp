#ifndef MINIQL_DEFAULTDENSITYSTRUCTURE_HPP
#define MINIQL_DEFAULTDENSITYSTRUCTURE_HPP

#include "../time/date.hpp"
#include "../defaulttermstructure.hpp"
#include "../typedef.hpp"

namespace MiniQL {

class DefaultDensityStructure : public DefaultProbabilityTermStructure {
  public:
    DefaultDensityStructure() = default;
    DefaultDensityStructure(const Date& referenceDate,
                            const Calendar& calendar = Calendar(),
                            const DayCounter& dc = DayCounter())
    : DefaultProbabilityTermStructure(referenceDate, calendar, dc) {}
    ~DefaultDensityStructure() = default;

  protected:
    Probability survivalProbabilityImpl(Time t) const override 
    {
      // need numerical integration -- e.g. Gaussian Quadrature
      
      // trapzoidal rule
      Size n = 100;
      Real dt = 1.0 * t / n;
      Probability prob = 0.0;
      prob += 0.5*defaultDensityImpl(0.0);
      for (Size i=1; i<n; ++i)
        prob += defaultDensityImpl(dt*i);
      prob += 0.5*defaultDensityImpl(t);
      prob *= dt;

      return std::max(1.0 - prob, 0.0);
    }
};

}  // namespace MiniQL

#endif