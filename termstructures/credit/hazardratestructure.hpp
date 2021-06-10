#ifndef MINIQL_HAZARDRATESTRUCTURE_HPP
#define MINIQL_HAZARDRATESTRUCTURE_HPP

#include <cmath>

#include "../time/date.hpp"
#include "../defaulttermstructure.hpp"
#include "../typedef.hpp"

namespace MiniQL {

class HazardRateStructure : public DefaultProbabilityTermStructure {
  public:
    HazardRateStructure() = default;
    HazardRateStructure(const Date& referenceDate,
                                 const Calendar& calendar = Calendar(),
                                 const DayCounter& dc = DayCounter())
    : DefaultProbabilityTermStructure(referenceDate, calendar, dc) {}
    ~HazardRateStructure() = default;

  protected:
    virtual Real hazardRateImpl(Time) const = 0;
    
    Probability survivalProbabilityImpl(Time t) const override 
    {
      // need numerical integration -- e.g. Gaussian Quadrature
      
      // trapzoidal rule
      Size n = 100;
      Real dt = 1.0 * t / n;
      Real accumu = 0.0;
      accumu += 0.5*hazardRateImpl(0.0);
      for (Size i=1; i<n; ++i)
        accumu += hazardRateImpl(dt*i);
      accumu += 0.5*hazardRateImpl(t);
      accumu *= dt;
      
      return std::exp( - accumu);
    }

    Real defaultDensityImpl(Time t) const override 
    {
      return hazardRateImpl(t) * survivalProbabilityImpl(t) ;
    }
};

}  // namespace MiniQL

#endif