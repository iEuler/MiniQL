#ifndef MINIQL_DEFAULTTERMSTURCTURE_HPP
#define MINIQL_DEFAULTTERMSTURCTURE_HPP

#include "../time/date.hpp"
#include "../termstructure.hpp"
#include "../typedef.hpp"


namespace MiniQL {

class DefaultProbabilityTermStructure : public TermStructure {
  public:
    DefaultProbabilityTermStructure() = default;
    DefaultProbabilityTermStructure(const Date& referenceDate,
                                    const Calendar& calendar = Calendar(),
                                    const DayCounter& dc = DayCounter())
    : TermStructure(referenceDate, calendar, dc) {}

    Probability survivalProbability(Time t) const
    { return survivalProbabilityImpl(t); }
    Real defaultDensity(Time t) const
    { return defaultDensityImpl(t); }
    
    Probability defaultProbability(Time t) const
    { return 1.0 - survivalProbability(t); }
    Probability defaultProbability(Time t1, Time t2) const
    { return defaultProbability(t2) - defaultProbability(t1); }
    Real hazardRate(Time t) const
    { 
      Probability survProb = survivalProbability(t);
      return (survProb < QL_EPSILON) ? survProb : defaultDensity(t)/survProb; 
    }

    // void update() override;


  protected:
    virtual Probability survivalProbabilityImpl(Time) const = 0;
    virtual Real defaultDensityImpl(Time) const = 0;

};


}  // namespace MiniQL

#endif
